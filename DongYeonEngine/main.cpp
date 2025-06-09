﻿#include "framework.h"
#include "Editor_Window.h"
#include "Game.h"
#include "LoadScenes.h"
#define IDB_BITMAP1 129
#define MAX_LOADSTRING 100


Game GAME;
// 전역 변수:
extern const UINT width = 1280;
extern const UINT height = 720;
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // 전역 문자열을 초기화합니다.
    wcscpy_s(szTitle, L"Wizard Of Legend"); // 창 제목을 "Wizard Of Legend"로 설정
    LoadStringW(hInstance, IDC_DONGYEONENGINE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DONGYEONENGINE));

    MSG msg;

    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;

            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            GAME.Run(); // 시작
        }
    }

    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;

    // 비트맵 로드 (16x16)
    HBITMAP hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
    if (!hBitmap)
    {
        MessageBox(NULL, L"Failed to load bitmap!", L"Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    // 비트맵 정보 확인
    BITMAP bmp;
    GetObject(hBitmap, sizeof(BITMAP), &bmp);
    if (bmp.bmWidth != 16 || bmp.bmHeight != 16)
    {
        MessageBox(NULL, L"Bitmap is not 16x16!", L"Warning", MB_OK | MB_ICONWARNING);
    }

    // 아이콘 정보 설정
    ICONINFO iconInfo = { 0 };
    iconInfo.fIcon = TRUE; // 아이콘으로 사용
    iconInfo.hbmColor = hBitmap;
    iconInfo.hbmMask = hBitmap; // 단순 마스크 (투명도 필요 시 별도 마스크 생성)

    // HICON 생성
    HICON hIcon = CreateIconIndirect(&iconInfo);
    if (!hIcon)
    {
        MessageBox(NULL, L"Failed to create icon!", L"Error", MB_OK | MB_ICONERROR);
        DeleteObject(hBitmap);
        return 0;
    }

    wcex.hIcon = hIcon; // 창 아이콘
    wcex.hIconSm = hIcon; // 작업 표시줄 아이콘 (16x16 적합)
    wcex.hCursor = LoadCursor(NULL, NULL); // 기본 커서
    wcex.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
    wcex.lpszMenuName = nullptr; // 메뉴바 제거
    wcex.lpszClassName = szWindowClass;

    ATOM result = RegisterClassExW(&wcex);

    // 리소스 정리
    DeleteObject(hBitmap);
    // HICON은 시스템이 관리하므로 DestroyIcon 호출하지 않음

    return result;
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr, hInstance, nullptr);
    GAME.Initialize(hWnd, width, height); // 애플리케이션 초기화

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    LoadScenes();

    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        ShowCursor(false);
        // 폰트 로드
        AddFontResourceEx(L"resources/Font/8bitWonder.ttf", FR_PRIVATE, nullptr);
        AddFontResourceEx(L"resources/Font/Exo2-Bold.ttf", FR_PRIVATE, nullptr);
        AddFontResourceEx(L"resources/Font/DungGeunMo.ttf", FR_PRIVATE, nullptr);
        AddFontResourceEx(L"resources/Font/PIXELMPLUS10-REGULAR.ttf", FR_PRIVATE, nullptr);
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