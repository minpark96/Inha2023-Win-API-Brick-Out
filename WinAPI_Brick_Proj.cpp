// WinAPI_Brick_Proj.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "WinAPI_Brick_Proj.h"

#define MAX_LOADSTRING 100

// >> : 사용자 정의 함수
#pragma comment(lib, "msimg32.lib")
RECT rectView;
HBITMAP hDoubleBufferImage, hDoubleBufferImage2;
void DrawDoubleBuffering(HWND hWnd, HDC hdc, CCircle& circle, CRectangle& pad, std::list<CRectangle>& bricks);
int score;
int lives = 3;
enum Scene { MAIN, PLAY, FAIL, SUCCESS };
Scene currentScene = MAIN;
// << :

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
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

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINAPIBRICKPROJ, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINAPIBRICKPROJ));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (true)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                break;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPIBRICKPROJ));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINAPIBRICKPROJ);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

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

#define timer_ID_1 1

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    using namespace std;
    PAINTSTRUCT ps;
    HDC hdc;
    static list<CRectangle> bricks;
    static CCircle circle;
    static CRectangle pad;
    static bool isleft = false;
    static bool isright = false;

    switch (message)
    {
    case WM_CREATE:
        GetClientRect(hWnd, &rectView);
        for (int i = 0; i < 11; i++)
        {
            bricks.push_back(CRectangle(200 + 100 * i, 100, 2, 6));
            bricks.push_back(CRectangle(200 + 100 * i, 150, 1, 5));
            bricks.push_back(CRectangle(200 + 100 * i, 200, 1, 4));
            bricks.push_back(CRectangle(200 + 100 * i, 250, 1, 3));
            bricks.push_back(CRectangle(200 + 100 * i, 300, 1, 2));
            bricks.push_back(CRectangle(200 + 100 * i, 350, 1, 1));
        }
        SetTimer(hWnd, timer_ID_1, 10, NULL);
        break;
    case WM_TIMER:
        if (currentScene == PLAY)
        {
            if (wParam == timer_ID_1)
            {
                int flag;

                for (auto it = bricks.begin(); it != bricks.end();)
                {
                    bool itErased = false;
                    flag = (*it).Collision(circle);

                    if (flag)
                    {
                        if ((*it).GetHp() == 0)
                        {
                            it = bricks.erase(it);
                            itErased = true;
                            score += 100;
                        }

                        circle.SetPosition(flag);
                    }

                    if (!itErased)
                    {
                        ++it;
                    }
                }

                flag = pad.Collision(circle);
                if (flag)
                {
                    circle.SetPosition(flag);
                }

                if (isleft)
                    pad.Update(&rectView, -3);
                if (isright)
                    pad.Update(&rectView, 3);

                if (circle.Update(&rectView))
                {
                    circle.Reset();
                    pad.Reset();
                    lives--;

                }
                InvalidateRect(hWnd, NULL, FALSE);

                if (lives == 0)
                {
                    currentScene = FAIL;
                    InvalidateRect(hWnd, NULL, TRUE);
                }
                if (bricks.empty())
                {
                    currentScene = SUCCESS;
                    InvalidateRect(hWnd, NULL, TRUE);
                }
            }
        }
        break;
    case WM_KEYDOWN:
    {
        switch (currentScene)
        {
        case MAIN:
            currentScene = PLAY;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case PLAY:
        {
            circle.Move();
            if (GetAsyncKeyState(VK_LEFT) & 0x8000)
            {
                isleft = true;
            }
            if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
            {
                isright = true;
            }
        }
            break;
        case FAIL:
            break;
        case SUCCESS:
            break;
        }
    }
    break;
    case WM_KEYUP:
    {
        if (currentScene == PLAY)
        {
            if (wParam == VK_LEFT)
            {
                isleft = false;
            }
            if (wParam == VK_RIGHT)
            {
                isright = false;
            }
        }
    }
    break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
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
    case WM_PAINT:
        {
            hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

            switch (currentScene)
            {
            case MAIN:
            {
                LPCWSTR text1 = L"BRICK OUT!";
                LPCWSTR text2 = L"PRESS ANY BUTTON...";
                TextOut(hdc, rectView.right / 2 - 30, rectView.bottom / 2 - 10, text1, wcslen(text1));
                TextOut(hdc, rectView.right / 2 - 60, rectView.bottom / 2 + 10, text2, wcslen(text2));
            }
                break;
            case PLAY:
                DrawDoubleBuffering(hWnd, hdc, circle, pad, bricks);
                break;
            case FAIL:
            {
                WCHAR textBuffer[10];
                wsprintf(textBuffer, L"%d", score);
                LPCWSTR text1 = L"실패입니다!";
                TextOut(hdc, rectView.right / 2 - 35, rectView.bottom / 2 - 10, text1, wcslen(text1));
                TextOut(hdc, rectView.right / 2 - 55, rectView.bottom / 2 + 10, _T("최종 스코어:"), _tcslen(_T("최종 스코어:")));
                TextOut(hdc, rectView.right / 2 + 45, rectView.bottom / 2 + 10, textBuffer, wcslen(textBuffer));
            }
                break;
            case SUCCESS:
            {
                WCHAR textBuffer[10];
                wsprintf(textBuffer, L"%d", score);
                LPCWSTR text1 = L"성공입니다!";
                TextOut(hdc, rectView.right / 2 - 35, rectView.bottom / 2 - 10, text1, wcslen(text1));
                TextOut(hdc, rectView.right / 2 - 55, rectView.bottom / 2 + 10, _T("최종 스코어:"), _tcslen(_T("최종 스코어:")));
                TextOut(hdc, rectView.right / 2 + 45, rectView.bottom / 2 + 10, textBuffer, wcslen(textBuffer));
            }
                break;
            }
            
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        KillTimer(hWnd, timer_ID_1);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
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

void DrawDoubleBuffering(HWND hWnd, HDC hdc, CCircle& circle, CRectangle& pad, std::list<CRectangle>& bricks)
{
    HDC hMemDC, hMemDC2;
    HBITMAP hOldBitmap, hOldBitmap2;
    HBRUSH hBrush, oldBrush;

    hMemDC = CreateCompatibleDC(hdc);
    if (hDoubleBufferImage == NULL)
    {
        hDoubleBufferImage = CreateCompatibleBitmap(hdc,
            rectView.right, rectView.bottom);
    }
    hOldBitmap = (HBITMAP)SelectObject(hMemDC, hDoubleBufferImage);

    hBrush = CreateSolidBrush(RGB(255, 255, 255));
    FillRect(hMemDC, &rectView, hBrush); // 배경색 채우기
    DeleteObject(hBrush);

    hMemDC2 = CreateCompatibleDC(hMemDC);
    if (hDoubleBufferImage2 == NULL)
    {
        hDoubleBufferImage2 = CreateCompatibleBitmap(hdc,
            rectView.right, rectView.bottom);
    }
    hOldBitmap2 = (HBITMAP)SelectObject(hMemDC2, hDoubleBufferImage2);

    hBrush = CreateSolidBrush(RGB(255, 255, 255));
    FillRect(hMemDC2, &rectView, hBrush); // 배경색 채우기
    DeleteObject(hBrush);

    WCHAR textBuffer[10];
    wsprintf(textBuffer, L"%d", score);
    TextOut(hMemDC2, 10, 10, _T("SCORE:"), _tcslen(_T("SCORE:")));
    TextOut(hMemDC2, 65, 10, textBuffer, wcslen(textBuffer));
    wsprintf(textBuffer, L"%d", lives);
    TextOut(hMemDC2, 1345, 10, _T("LIVES:"), _tcslen(_T("LIVES:")));
    TextOut(hMemDC2, 1390, 10, textBuffer, wcslen(textBuffer));

    hBrush = CreateSolidBrush(RGB(0, 255, 0));
    oldBrush = (HBRUSH)SelectObject(hMemDC2, hBrush);
    circle.Draw(hMemDC2);
    SelectObject(hMemDC2, oldBrush);
    DeleteObject(hBrush);

    hBrush = CreateSolidBrush(RGB(135, 206, 235));
    oldBrush = (HBRUSH)SelectObject(hMemDC2, hBrush);
    pad.Draw(hMemDC2);
    SelectObject(hMemDC2, oldBrush);
    DeleteObject(hBrush);

    for (auto brick : bricks)
    {
        hBrush = CreateSolidBrush(brick.GetColor());
        oldBrush = (HBRUSH)SelectObject(hMemDC2, hBrush);
        brick.Draw(hMemDC2);
        SelectObject(hMemDC2, oldBrush);
        DeleteObject(hBrush);
    }

    BitBlt(hMemDC, 0, 0, rectView.right, rectView.bottom, hMemDC2, 0, 0, SRCCOPY);
    SelectObject(hMemDC2, hOldBitmap2);
    DeleteDC(hMemDC2);

    BitBlt(hdc, 0, 0, rectView.right, rectView.bottom, hMemDC, 0, 0, SRCCOPY);
    SelectObject(hMemDC, hOldBitmap);
    DeleteDC(hMemDC);
}