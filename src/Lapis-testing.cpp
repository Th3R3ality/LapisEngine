// Lapis-testing.cpp : This file contains the 'main' function. Program execution begins and ends there.

// include the basic windows header files and the Direct3D header files
#include <windows.h>
#include <windowsx.h>
#include <chrono>
#include <iostream>

#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <d3dcompiler.h>

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#include "Backend.h"
#include "DataTypes.hpp"

#include "utils/hsl-to-rgb.hpp"


using namespace Lapis;
LapisInstance engine;


// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    
    HWND hwnd;
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_NO);
    wc.lpszClassName = L"LapisWindowClass";

    // register the window class
    RegisterClassEx(&wc);

    RECT wr = { 0, 0, 800, 600 };

    // create the window and use the result as the handle
    hwnd = CreateWindowExW(NULL,
        wc.lpszClassName,    // name of the window class
        L"Lapis Dev Window",   // title of the window
        WS_OVERLAPPEDWINDOW,    // window style
        300,    // x-position of the window
        300,    // y-position of the window
        wr.right -wr.left,    // width of the window
        wr.bottom -wr.top,    // height of the window
        NULL,    // we have no parent window, NULL
        NULL,    // we aren't using menus, NULL
        hInstance,    // application handle
        NULL);    // used with multiple windows, NULL
    ShowWindow(hwnd, nCmdShow);

    AllocConsole();
    SetConsoleTitleW(L"DEBUG OUTPUT");
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    printf("negawatt!\n");

    engine.Init();
    engine.InitD3D11(hwnd);

    auto start = std::chrono::high_resolution_clock::now().time_since_epoch();
    auto old = start;

    MSG msg;
    while (true)
    {
        auto temp = std::chrono::high_resolution_clock::now().time_since_epoch();
        engine.deltaDuration = (temp - old);
        engine.elapsedDuration = (temp - start);
        old = temp;

        engine.deltaTime = (float)std::chrono::duration_cast<std::chrono::microseconds>(engine.deltaDuration).count() / 1000 / 100;
        engine.elapsedTime += engine.deltaTime;
        std::cout << "delta: " << engine.deltaTime * 100 << "ms\t";

        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
                break;
        }
        static float x = 100;
        static float y = 75;
        float moveDistance = engine.deltaTime * 5;
        if (GetAsyncKeyState(VK_LEFT)) x -= moveDistance;
        if (GetAsyncKeyState(VK_RIGHT)) x += moveDistance;
        if (GetAsyncKeyState(VK_UP)) y += moveDistance;
        if (GetAsyncKeyState(VK_DOWN)) y -= moveDistance;
        std::cout << "x: " << x << " - y: " << y << "\n";
        

        engine.DrawPoint(((float)(int)x - 100) / 100 - 0.5/100, ((float)(int)y-75) / 75 - 0.5/75);

        float lineLength = (sinf(engine.elapsedTime * 0.1) + 1) / 3 ;
        engine.DrawLine(
            lineLength * sinf(engine.elapsedTime*.5),
            lineLength * cosf(engine.elapsedTime*.5),
            lineLength * -sinf(engine.elapsedTime*0.5),
            lineLength * -cosf(engine.elapsedTime*0.5));

        
        engine.DrawRect(
            -0.5 + sinf(engine.elapsedTime * 0.1),
            -0.55 + (sinf(engine.elapsedTime * 0.4 + 2) + 1) / 2 * 0.4, 
            0.8, 
            0.01 + (sinf(engine.elapsedTime * 0.3)+1) / 2 * 0.3,
            HSLToRGB((sinf(engine.elapsedTime*0.2 + 1)+1)/2 * 360, 1.f, 0.7f, 1));

        engine.RenderFrame();
        engine.CleanFrame();
    }

    std::cout << "Cleaning up";
    engine.CleanD3D11();

    return msg.wParam;
}

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // sort through and find what code to run for the message given
    switch (message) {
        // this message is read when the window is closed
    case WM_DESTROY:
        // close the application entirely
        PostQuitMessage(0);
        return 0;
        break;
    }
    // Handle any messages the switch statement didn't
    return DefWindowProc(hWnd, message, wParam, lParam);
}