// lab8.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "lab8.h"

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

HBITMAP  g_hBmp, g_hBmp2;

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
    LoadStringW(hInstance, IDC_LAB8, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    g_hBmp = (HBITMAP)::LoadImage(hInstance, TEXT("paint.bmp"), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_LOADFROMFILE);
    g_hBmp2 = (HBITMAP)::LoadImage(hInstance, TEXT("clear.bmp"), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_LOADFROMFILE);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB8));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB8));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB8);
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

#define ID_DRAW 228
#define ID_CLEAR 229

HWND hButtonDraw, hButtonClear;

void InitWindow(HWND hWnd)
{
    hButtonDraw = CreateWindowW(L"Button", L"Добавить", WS_CHILD | BS_OWNERDRAW | WS_VISIBLE, 20, 20, 100, 100, hWnd, (HMENU)ID_DRAW, NULL, NULL);
    hButtonClear = CreateWindowW(L"Button", L"Отчистить", WS_CHILD | BS_OWNERDRAW | WS_VISIBLE, 200, 20, 100, 100, hWnd, (HMENU)ID_CLEAR, NULL, NULL);
}

SIZE SizeBitmap(HBITMAP hBmp) {
    SIZE resSize = { 0,0 };
    BITMAP bmp = { 0,0,0,0,0,0,NULL };
    int iRes = ::GetObject(hBmp, sizeof(BITMAP), &bmp);
    if (iRes == sizeof(BITMAP)) {
        resSize.cx = bmp.bmWidth;
        resSize.cy = bmp.bmHeight;
    }
    return resSize;
}

bool drawMode = false;


void Line(HDC hdc, int x1, int y1, int x2, int y2) {
    MoveToEx(hdc, x1, y1, NULL);
    LineTo(hdc, x2, y2);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        InitWindow(hWnd);
        break;
    case WM_DRAWITEM: {
        DRAWITEMSTRUCT* lpDrawItem = (DRAWITEMSTRUCT*)lParam;
        int wmId = LOWORD(wParam);
        
        if (wmId == ID_DRAW) {
            HDC hCDC = ::CreateCompatibleDC(lpDrawItem->hDC);
            HBITMAP hBmp = g_hBmp;
            HBITMAP hBmpOld = (HBITMAP)::SelectObject(hCDC, hBmp);

            ::StretchBlt(
                lpDrawItem->hDC,
                0, 0, lpDrawItem->rcItem.right - lpDrawItem->rcItem.left, lpDrawItem->rcItem.bottom - lpDrawItem->rcItem.top,
                hCDC,
                0, 0, SizeBitmap(hBmp).cx, SizeBitmap(hBmp).cy,
                SRCCOPY
            );


            ::SelectObject(hCDC, hBmpOld);
            ::DeleteDC(hCDC);
        }
        else {
            HDC hCDC = ::CreateCompatibleDC(lpDrawItem->hDC);
            HBITMAP hBmp = g_hBmp2;
            HBITMAP hBmpOld = (HBITMAP)::SelectObject(hCDC, hBmp);

            ::StretchBlt(
                lpDrawItem->hDC,
                0, 0, lpDrawItem->rcItem.right - lpDrawItem->rcItem.left, lpDrawItem->rcItem.bottom - lpDrawItem->rcItem.top,
                hCDC,
                0, 0, SizeBitmap(hBmp).cx, SizeBitmap(hBmp).cy,
                SRCCOPY
            );

            ::SelectObject(hCDC, hBmpOld);
            ::DeleteDC(hCDC);
        }
        break;
    }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case ID_DRAW:
                drawMode = true;
                InvalidateRect(hWnd, NULL, false);
                break;
            case ID_CLEAR:
                drawMode = false;
                InvalidateRect(hWnd, NULL, false);
                break;
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

            if (drawMode) {
                int X = 400, Y = 50;

                HBRUSH  brush = CreateSolidBrush(RGB(254, 255, 220));
                SelectObject(hdc, brush);
                Rectangle(hdc, X, Y + 260, X + 130, Y + 400);
                DeleteObject(brush);

                /*ТРЕУГОЛЬНАЯ КРЫША*/
                POINT poly[3] = { {X,Y + 260 },{ X + 130 , Y + 260 },{X + 130 / 2, Y + 200} };
                brush = CreateHatchBrush(HS_DIAGCROSS, RGB(199, 214, 255));
                SelectObject(hdc, brush);
                Polygon(hdc, poly, 3);
                DeleteObject(brush);

                /*БОК КРЫШИ ПАРАЛЛЕЛОГРАММ*/
                POINT poly2[4] = { {X + 65,Y + 200 },{X + 280 , Y + 200 },{X + 345,Y + 260}, {X + 130, Y + 260 } };
                brush = CreateHatchBrush(HS_HORIZONTAL, RGB(0, 0, 255));
                SelectObject(hdc, brush);
                Polygon(hdc, poly2, 4);
                DeleteObject(brush);

                /*БОК ДОМА*/
                brush = CreateHatchBrush(7, RGB(200, 255, 200));
                SelectObject(hdc, brush);
                Rectangle(hdc, X + 345, Y + 260, X + 130, Y + 400);
                DeleteObject(brush);

                /*ДВЕРЬ*/
                brush = CreateSolidBrush(RGB(175, 75, 0));
                SelectObject(hdc, brush);
                Rectangle(hdc, X + 70, Y + 320, X + 110, Y + 400);
                DeleteObject(brush);

                /*ОКНА*/
                brush = CreateHatchBrush(HS_BDIAGONAL, RGB(0, 255, 0));
                SelectObject(hdc, brush);
                Rectangle(hdc, X + 150, Y + 300, X + 190, Y + 340);
                Rectangle(hdc, X + 220, Y + 300, X + 260, Y + 340);
                DeleteObject(brush);

                brush = CreateSolidBrush(RGB(200, 200, 200));
                SelectObject(hdc, brush);
                Rectangle(hdc, X + 290, Y + 300, X + 330, Y + 340);
                DeleteObject(brush);

                X = 800;
                Y = 300;


                brush = CreateSolidBrush(RGB(0, 255, 0));
                SelectObject(hdc, brush);
                Line(hdc, X, Y, X, Y - 150); //Здесь 150 - это высота ствола, рисование идёт снизу к верху.

                /*Рисуем ветки*/
                Line(hdc, X, Y - 75, X - 50, Y - 75 - 15);
                //circle == (X,Y, X+R, Y+R)
                Ellipse(hdc, X - 70, Y - 105, X - 40, Y - 105 + 30); //Подгоняем листья

                Line(hdc, X, Y - 95, X - 50, Y - 95 - 15);
                Ellipse(hdc, X - 75, Y - 120, X - 50, Y - 120 + 15); //Подгоняем листья
                Ellipse(hdc, X - 50, Y - 130, X - 35, Y - 120 + 15); //Подгоняем листья

                Ellipse(hdc, X - 10, Y - 150, X + 10, Y - 150 + 15); //Подгоняем листья

                Line(hdc, X, Y - 60, X + 80, Y - 165);  //Ветви
                Line(hdc, X, Y - 90, X + 30, Y - 135);
                Line(hdc, X + 25, Y - 95, X + 85, Y - 100);
                Line(hdc, X + 50, Y - 130, X + 55, Y - 170);

                Ellipse(hdc, 15 + X + 55, 15 + Y - 170, 15 + X + 75, 15 + Y - 190); //листва
                Ellipse(hdc, 15 + X + 34, 15 + Y - 180, 15 + X + 46, 15 + Y - 200);
                Ellipse(hdc, 25 + X + (-5), 25 + Y - 120, 25 + X + 7, 25 + Y - 144);
                Ellipse(hdc, 15 + X + 55, 15 + Y - 103, 15 + X + 75, 15 + Y - 127);
                Ellipse(hdc, 15 + X + 10, 15 + Y - 140, 15 + X + 28, 15 + Y - 158);

                DeleteObject(brush);
            }
            else {
                SelectObject(hdc, (HBRUSH)(COLOR_WINDOW + 1));
                Rectangle(hdc, 400, 50, 900, 550);
            }

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
