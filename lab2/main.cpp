#include <iostream>
#include <stdlib.h>
#include <locale.h>  
#include <stdio.h>
#include <windows.h>
#include <tchar.h>


void display_error_code(LPTSTR f) {
    LPVOID message_buffer;
    LPVOID display_buffer;
    DWORD dw = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&message_buffer,
        0, 
        NULL
    );

    display_buffer = (LPVOID)LocalAlloc(
        LMEM_ZEROINIT,
        (lstrlen((LPCTSTR)message_buffer) + lstrlen((LPCTSTR)f) + 40) * sizeof(TCHAR)
    );

    wprintf(
        L"%sfailed with error %d: %s",
        f, 
        dw, 
        message_buffer
    );

    LocalFree(message_buffer);
    LocalFree(display_buffer);
}

BOOL find_file_by_cluster_number(TCHAR* location, LONGLONG cluster) {
    HANDLE handle_device = CreateFile(
        location,      
        GENERIC_READ | GENERIC_WRITE,                       
        FILE_SHARE_READ | FILE_SHARE_WRITE,  
        NULL,                                
        OPEN_EXISTING,                       
        FILE_FLAG_BACKUP_SEMANTICS,          
        NULL
    );

    if (handle_device == INVALID_HANDLE_VALUE) {
        display_error_code(LPSTR("CreateFile"));
        return FALSE;
    }

    LOOKUP_STREAM_FROM_CLUSTER_INPUT input = { 0 };
    input.NumberOfClusters = 1;
    input.Cluster[0].QuadPart = cluster;
    input.Flags = 0;

    BYTE output[5000] = {};
    DWORD dw_result = 0;
    LOOKUP_STREAM_FROM_CLUSTER_OUTPUT result = { 0 };

    BOOL bool_result = DeviceIoControl(
        (HANDLE)handle_device,   
        FSCTL_LOOKUP_STREAM_FROM_CLUSTER, 
        (LPVOID)&input,        
        (DWORD)sizeof(input),     
        (LPVOID)output,       
        (DWORD)5000,    
        (LPDWORD)&dw_result,   
        NULL
    );    

    if (bool_result == FALSE) {
        display_error_code(LPSTR("DeviceIoControl"));
        return FALSE;
    }

    memcpy(&result, output, sizeof(LOOKUP_STREAM_FROM_CLUSTER_OUTPUT));

    if (result.NumberOfMatches == 0) {
        wprintf(L"File not found\n");
        return FALSE;
    }

    wprintf(L"Information about file\n");

    BYTE* p = (BYTE*)output + result.Offset;
    LOOKUP_STREAM_FROM_CLUSTER_ENTRY* pentry = (LOOKUP_STREAM_FROM_CLUSTER_ENTRY*)p;

    wprintf(L"Flags: 0x%x ", (UINT)pentry->Flags);

    if ((pentry->Flags & LOOKUP_STREAM_FROM_CLUSTER_ENTRY_FLAG_PAGE_FILE) > 0) 
        wprintf(L"(Pagefile)");
    else if ((pentry->Flags & LOOKUP_STREAM_FROM_CLUSTER_ENTRY_FLAG_FS_SYSTEM_FILE) > 0)  
        wprintf(L"(Internal filesystem file)");
    else if ((pentry->Flags & LOOKUP_STREAM_FROM_CLUSTER_ENTRY_FLAG_TXF_SYSTEM_FILE) > 0) 
        wprintf(L"(Internal TXF file)");
    else 
        wprintf(L"(Normal file)");

    wprintf(L"\nFile: %s\n", pentry->FileName);

    return TRUE;
}

int _tmain(int argc, _TCHAR* argv[]) {
    setlocale( LC_ALL, "en-US" );

    LONGLONG cluster_number = 0;
    wprintf(L"Input cluster number: \n");
    scanf_s("%llu", &cluster_number);

    find_file_by_cluster_number(LPSTR("\\\\.\\C:"), cluster_number);

    system("PAUSE");
    return 0;
}
