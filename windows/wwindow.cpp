//
// Created by hakeee on 2015-09-06.
//

#include "wwindow.h"
#include "../generic/input.h"
#include "wdgraphics.h"
#include "wggraphics.h"

using namespace discobreak;

LRESULT Wwindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

    switch (message) {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_MOUSEMOVE:
            break;
        case WM_KEYUP:
            Input::SetKeyState((char)wParam, false);
            break;
        case WM_KEYDOWN:
            Input::SetKeyState((char)wParam, true);
                    break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}



Wwindow::Wwindow(std::string name, GRAPHICS graphics) {
    std::wstring wStringName = std::wstring(name.begin(), name.end());

    hInstance_ = GetModuleHandle(NULL);

    WNDCLASSEX wndcex;
    wndcex.cbSize = sizeof(WNDCLASSEX);
    wndcex.style = CS_HREDRAW | CS_VREDRAW;
    wndcex.lpfnWndProc = WndProc;
    wndcex.cbClsExtra = 0;
    wndcex.cbWndExtra = 0;
    wndcex.hInstance = hInstance_;
    wndcex.hIcon = NULL;
    wndcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wndcex.lpszMenuName = NULL;
    wndcex.lpszClassName = "WindowClass";// wStringName.c_str();
    wndcex.hIconSm = 0;

    if (!RegisterClassEx(&wndcex)) {
        MessageBox(NULL, "Failed to register window class\nWindow::Window(std::string name)", "Error", MB_ICONERROR);
        return;
    }

    RECT windowRect = { 0, 0, windowWidth_, windowHeight_ };
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);

    hWnd_ = CreateWindow(
            "WindowClass",
            name.c_str(),
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            windowRect.right - windowRect.left,
            windowRect.bottom - windowRect.top,
            NULL,
            NULL,
            hInstance_,
            NULL);

    if (!hWnd_) {
        MessageBox(0, "CreateWindow failed\nWindow::Window(std::string name)", "Error", MB_ICONERROR);
        return;
    }

    ShowWindow(hWnd_, 1);

	if(graphics == GRAPHICS::DIRECTX)
		graphics_ = new Graphics(new Wdgraphics(this));
	else
		graphics_ = new Graphics(new Wggraphics(this));
}

Wwindow::~Wwindow() {
	if (graphics_ != nullptr)
		delete graphics_;
}

void Wwindow::Run() {

	//Temp timer
	__int64 cntsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);
	double secsPerCnt = 1.0f / (double)cntsPerSec;

	__int64 prevTimeStamp = 0;

    // Main message loop
    MSG msg = { 0 };
    while (WM_QUIT != msg.message) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
			__int64 currTimeStamp = 0;
			QueryPerformanceCounter((LARGE_INTEGER*)&currTimeStamp);
			//dt i ms
			double dt = (currTimeStamp - prevTimeStamp) * secsPerCnt * 1000;

			if (update_callback_)
				update_callback_(dt);
			if (draw_callback_)
				draw_callback_(dt);

			Input::PostUpdate();
			prevTimeStamp = currTimeStamp;
        }
    }
}

int Wwindow::GetScreenHeight() {
	RECT rect;
	GetClientRect(hWnd_, &rect);
	return rect.bottom;
}

int Wwindow::GetScreenWidth() {
	RECT rect;
	GetClientRect(hWnd_, &rect);
	return rect.right;
}

