// lab13.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "lab13.h"
#include <windowsx.h>
#include <string.h>
#include <Psapi.h>
#include <scoped_allocator>
#include <string>
#include <vector>

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB13, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB13));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB13));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB13);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//

#define PROCESSES_LIST 89
#define MODULES_LIST  90

HWND processesList;
HWND modulesList;

void init(HWND hWnd) {
    processesList  = CreateWindow(
        L"listbox", L"processes", 
        WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | WS_BORDER | LBS_NOTIFY | WS_VSCROLL,
        40, 40, 900, 500, 
        hWnd, (HMENU)PROCESSES_LIST, 
        hInst, NULL
    );

    modulesList = CreateWindow(
        L"listbox", L"modules",
        WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | WS_BORDER | LBS_NOTIFY | WS_VSCROLL,
        940, 40, 400, 500, 
        hWnd, (HMENU)MODULES_LIST, 
        hInst, NULL
    );
}

DWORD processesCount;
DWORD processIds[1024];

#include <string>

using namespace std;

wstring GetPriority(DWORD id) {
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_SET_INFORMATION, FALSE, id);
    DWORD priority = GetPriorityClass(hProcess);

    switch (priority) {
    case ABOVE_NORMAL_PRIORITY_CLASS:
        return L"Above normal";
    case BELOW_NORMAL_PRIORITY_CLASS:
        return L"Below normal";
    case HIGH_PRIORITY_CLASS:
        return L"High";
    case IDLE_PRIORITY_CLASS:
        return L"Idle";
    case NORMAL_PRIORITY_CLASS:
        return L"Normal";
    case REALTIME_PRIORITY_CLASS:
        return L"Realtime";
    }
}

void AddToList(HWND hList, const wstring& text) {
    SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)text.c_str());
}

void initProcessesList() {
    DWORD bytesNeeded;
    SendMessage(processesList, LB_RESETCONTENT, NULL, NULL);
    DWORD idsData[1024];
    EnumProcesses(idsData, sizeof(idsData), &bytesNeeded);
    processesCount = bytesNeeded / sizeof(DWORD);
    int size = 0;
    wstring name;

    for (int i = 0; i < processesCount; i++) {
        {
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, idsData[i]);
            if (hProcess == NULL) {
                continue;
            }
            else {
                TCHAR buffer[MAX_PATH];
                GetModuleFileNameEx(hProcess, NULL, buffer, MAX_PATH);
                wstring fullName = wstring(buffer);
                //name = fullName.substr(fullName.rfind(_T("\\")) + 1, fullName.length());
                name = fullName;
            }
        }

        processIds[size] = idsData[i];
        TCHAR itemString[1024];
        for (int i = 0; i < 1024; i++) {
            itemString[i] = 0;
        }
        wstring priority = GetPriority(processIds[size]);
        swprintf_s(itemString, _T("%ls (%d) - %ls"), name.c_str(), processIds[size], priority.c_str());
        AddToList(processesList, wstring(itemString));
        size++;
    }
    processesCount = size;
}

int GetSelectedIndex(HWND hList) {
    return SendMessage(hList, LB_GETCURSEL, (WPARAM)(0), (LPARAM)(0));
}

int SelectedProcess() {
    int selectedItem = GetSelectedIndex(processesList);
    if (selectedItem == LB_ERR) {
        return -1;
    }
    return processIds[selectedItem];
}

void ClearList(HWND hList) {
    SendMessage(hList, LB_RESETCONTENT, 0, 0);
}
DWORD bytesNeeded;
TCHAR moduleName[4096];
HMODULE hModules[4096];

vector<wstring> GetProcessModules(int pid) {
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    vector<wstring> result;

	if (hProcess != NULL) {
		if (EnumProcessModulesEx(hProcess, hModules, sizeof(hModules), &bytesNeeded, LIST_MODULES_ALL)) {
			int countOfModules = bytesNeeded / sizeof(DWORD);
			for (int i = 1; i < countOfModules; i++) {
				GetModuleBaseName(hProcess, hModules[i], moduleName, sizeof(moduleName) / sizeof(TCHAR));
				wstring moduleNameStr(moduleName);
				if (moduleNameStr == _T(""))
					moduleNameStr = _T("noname");
				result.push_back(wstring(moduleName));
			}
        }
        else {
            int  x = GetLastError();
            int y = x;
        }
	}
	CloseHandle(hProcess);
	return result;
}

void GetModules() {
    int cur = SelectedProcess();
    if (cur == -1) {
        return;
    }
    ClearList(modulesList);
    vector<wstring> res = GetProcessModules(cur);  
    for (int i = 0; i < res.size(); i++) {
        AddToList(modulesList, res[i]);
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:{
            init(hWnd);
            initProcessesList();
            break;
        }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case PROCESSES_LIST: {
                GetModules();
                break;
            }
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
