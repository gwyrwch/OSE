// lab11.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "lab11.h"

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
    LoadStringW(hInstance, IDC_LAB11, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB11));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB11));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB11);
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

HANDLE threads[6];
double x[6], y[6];
int step[6];
int CC = 1000;

bool finish = false;
#include <cmath>

const double PI = std::acos(-1);

DWORD WINAPI f1(CONST LPVOID lpParam) {
    int i = 0;
    while (!finish) {
        (step[i] += 1) %= CC;
        x[i] = std::sin(1. * step[i] / (CC - 1) * 2 * PI) * 50;
        y[i] = std::cos(1. * step[i] / (CC - 1) * 2 * PI) * 50;
        Sleep(10);
    }
    return 0;
}
DWORD WINAPI f2(CONST LPVOID lpParam) {
    int i = 1;
    while (!finish) {
        (step[i] += 1) %= CC;
        x[i] = std::sin(1. * step[i] / (CC - 1) * 2 * PI) * 50;
        y[i] = std::cos(1. * step[i] / (CC - 1) * 2 * PI) * 50;
        Sleep(10);
    }
    return 0;
}
DWORD WINAPI f3(CONST LPVOID lpParam) {
    int i = 2;
    while (!finish) {
        (step[i] += 1) %= CC;
        x[i] = std::sin(1. * step[i] / (CC - 1) * 2 * PI) * 50;
        y[i] = std::cos(1. * step[i] / (CC - 1) * 2 * PI) * 50;
        Sleep(10);
    }
    return 0;
}
DWORD WINAPI f4(CONST LPVOID lpParam) {
    int i = 3;
    while (!finish) {
        (step[i] += 1) %= CC;
        x[i] = std::sin(1. * step[i] / (CC - 1) * 2 * PI) * 50;
        y[i] = std::cos(1. * step[i] / (CC - 1) * 2 * PI) * 50;
        Sleep(10);
    }
    return 0;
}
DWORD WINAPI f5(CONST LPVOID lpParam) {
    int i = 4;
    while (!finish) {
        (step[i] += 1) %= CC;
        x[i] = std::sin(1. * step[i] / (CC - 1) * 2 * PI) * 50;
        y[i] = std::cos(1. * step[i] / (CC - 1) * 2 * PI) * 50;
        Sleep(10);
    }
    return 0;
}

DWORD WINAPI f6(CONST LPVOID lpParam) {
    int i = 5;
    while (!finish) {
        (step[i] += 1) %= CC;
        x[i] = std::sin(1. * step[i] / (CC - 1) * 2 * PI) * 50;
        y[i] = std::cos(1. * step[i] / (CC - 1) * 2 * PI) * 50;
        Sleep(10);
    }
    return 0;
}

void CreateThreads() {
    for (int i = 0; i < 6; i++) {
        step[i] = rand() % CC;
    }

    threads[0] = CreateThread(NULL, 0, &f1, NULL, 0, NULL);
    threads[1] = CreateThread(NULL, 0, &f2, NULL, 0, NULL);
    threads[2] = CreateThread(NULL, 0, &f3, NULL, 0, NULL);
    threads[3] = CreateThread(NULL, 0, &f4, NULL, 0, NULL);
    threads[4] = CreateThread(NULL, 0, &f5, NULL, 0, NULL);
    threads[5] = CreateThread(NULL, 0, &f6, NULL, 0, NULL);
}

void ResumeThreads() {
    for (int i = 0; i < 6; i++) {
        ResumeThread(threads[i]);
    }
}

#define TIMER_ID 228

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE: {
        CreateThreads();
        SetTimer(hWnd, TIMER_ID, 50, NULL);
        break;
    }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
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
    case WM_TIMER:
        InvalidateRect(hWnd, NULL, true);
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            for (int i = 0; i < 6; i++) {
                HBRUSH b = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
                SelectObject(hdc, b);

                int x0 = 100 + i * 100, y0 = 100;
                Ellipse(hdc, x[i] - 5 + x0, y[i] - 5 + y0, x[i] + 5 + x0, y[i] + 5 + y0);
            }

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        finish = true;
        KillTimer(hWnd, TIMER_ID);
        PostQuitMessage(0);
        for (int i = 0; i < 6; i++) {
            CloseHandle(threads[i]);
        }
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
