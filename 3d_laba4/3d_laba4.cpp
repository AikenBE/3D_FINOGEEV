// 3d_laba4.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "3d_laba4.h"
#include "math.h"

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
    LoadStringW(hInstance, IDC_MY3DLABA4, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY3DLABA4));

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

    return (int)msg.wParam;
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

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY3DLABA4));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MY3DLABA4);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static double f = 0.3, theta = -1.57, R = 20;

    int pyramid[12][3] = {
        {-1, 8, -2}, {-2, 8, 0}, {-1, 8, 2}, {1, 8, 2}, {2, 8, 0}, {1, 8, -2},
        {-2, 0, -4}, {-4, 0, 0}, {-2, 0, 4}, {2, 0, 4}, {4, 0, 0}, {2, 0, -4},
    };

    int edges[][2] = {
        {0,1}, {1,2}, {2,3}, {3,4}, {4,5}, {5,0}, // Верхнее основание
        {6,7}, {7,8}, {8,9}, {9,10}, {10,11}, {11,6}, // Нижнее основание
        {0,6}, {1,7}, {2,8}, {3,9}, {4,10}, {5,11}  // Боковые ребра
    };

    POINT screen_points[12];
    POINT screen_points_ortho[12];
    double view_transformation[12][4];
    double final_coords[12][4];
    double final_coords_ortho[12][4];

    int width = 800, lenght = 800, object = 10000;
    int width_ortho = 200, lenght_ortho = 200, object_ortho = 10500;

    double specific[4][4] = {
        {-sin(theta), -cos(f) * cos(theta), -sin(f) * cos(theta) , 0},
        {cos(theta), -cos(f) * sin(theta), -sin(f) * cos(theta) , 0},
        {0, sin(f), -cos(f), 0},
        {0, 0, R, 1},
    };

    double d = R * (width * lenght / object);
    double z = R * (width_ortho * lenght_ortho / object_ortho);

    double projection[4][4] = {
     {d,  0,  0, 0}, // x
     {0,  -d,  0, 0}, // -y
     {0,  0,  1, 1},// z w
     {400, 600, 0, 1}
    };

    double projection_ortho[4][4] = {
    {z,  0,  0, 0},
    {0, -z,  0, 0},
    {0,   0,  0, 0},
    {700, 600, 0, 1} 
    };

    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 4; j++) {
            view_transformation[i][j] =
                pyramid[i][0] * specific[0][j] +
                pyramid[i][1] * specific[1][j] +
                pyramid[i][2] * specific[2][j] +
                1.0 * specific[3][j];
        }

        for (int j = 0; j < 4; j++) {
            final_coords[i][j] =
                view_transformation[i][0] * projection[0][j] +
                view_transformation[i][1] * projection[1][j] +
                view_transformation[i][2] * projection[2][j] +
                view_transformation[i][3] * projection[3][j];
        }

        for (int j = 0; j < 4; j++) {
            final_coords_ortho[i][j] =
                view_transformation[i][0] * projection_ortho[0][j] +
                view_transformation[i][1] * projection_ortho[1][j] +
                view_transformation[i][2] * projection_ortho[2][j] +
                view_transformation[i][3] * projection_ortho[3][j];
        }

        double w = final_coords[i][3];
        screen_points[i].x = (int)(final_coords[i][0] / w + 500);
        screen_points[i].y = (int)(final_coords[i][1] / w + 700);

        double w_o = final_coords_ortho[i][3];
        screen_points_ortho[i].x = (int)(final_coords_ortho[i][0] / w_o);
        screen_points_ortho[i].y = (int)(final_coords_ortho[i][1] / w_o);
    }

    switch (message)
    {
    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case 'W':
            f = f + 0.005;
            break;
        case 'S':
            f = f - 0.005;
            break;
        case 'A':
            theta = theta - 0.005;
            break;
        case 'D':
            theta = theta + 0.005;
            break;
        case 'Q':
            R = R - 2.0;
            break;
        case 'E':
            R = R + 2.0;
            break;
        }

        if (R < 2.0) R = 2.0;

        InvalidateRect(hWnd, NULL, TRUE);
        break;
    }

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
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
        break;
    }

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        for (int i = 0; i < 18; i++) {

            int n1 = edges[i][0];
            int n2 = edges[i][1];

            //MoveToEx(hdc, screen_points[n1].x, screen_points[n1].y, NULL);
            //LineTo(hdc, screen_points[n2].x, screen_points[n2].y);

            MoveToEx(hdc, screen_points_ortho[n1].x, screen_points_ortho[n1].y, NULL);
            LineTo(hdc, screen_points_ortho[n2].x, screen_points_ortho[n2].y);
        }

        EndPaint(hWnd, &ps);
        break;
    }

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
