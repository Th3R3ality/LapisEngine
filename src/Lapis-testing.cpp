// Lapis-testing.cpp : This file contains the 'main' function. Program execution begins and ends there.

// include the basic windows header files
#include <windows.h>
#include <windowsx.h>

// include chrono for time
#include <chrono>
#include <thread>

// include iostream for printing
#include <iostream>

// include DirectX headers
#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <d3dcompiler.h>

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

// include Lapis headers
#include "Backend.h"
#include "DataTypes.hpp"
#include "utils/hsl-to-rgb.hpp"


using namespace Lapis;
LapisInstance engine;


// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    
    // define window class
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
    HWND hwnd;

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
    ShowWindow(hwnd, nCmdShow); // make sure window is shown

#ifdef _DEBUG
    AllocConsole();
    SetConsoleTitleW(L"DEBUG OUTPUT");
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    printf("yoo!\n");
#endif

    printf("initting lapis\n");
    engine.Init();
    printf("initting d3d11\n");
    engine.InitD3D11(hwnd);

    float FPS_CAP = 60;
    MSG msg;
    while (true)
    {

        if (GetAsyncKeyState(VK_INSERT) & 0x1) {
            auto hmodule = LoadLibraryW(L"C:\\Users\\reality\\source\\repos\\present hook\\present hook\\bin\\present hook_Debug.dll");
        }

        engine.NewFrame();
        std::cout << "delta: " << engine.deltaTime * 100 << "ms\n";
        
        

        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
                break;
        }

        static float x = 0;
        static float y = 0;
        static float z = 0;
        float moveDistance = 3 * engine.deltaTime;

        if (GetAsyncKeyState('A')) x += moveDistance;
        if (GetAsyncKeyState('D')) x -= moveDistance;
        if (GetAsyncKeyState('Q')) y += moveDistance;
        if (GetAsyncKeyState('E')) y -= moveDistance;
        if (GetAsyncKeyState('W')) z -= moveDistance;
        if (GetAsyncKeyState('S')) z += moveDistance;

        engine.cameraPosition = { x,y,z };

        if (GetAsyncKeyState(VK_RIGHT)) engine.CameraRotationY -= moveDistance;
        if (GetAsyncKeyState(VK_LEFT)) engine.CameraRotationY += moveDistance;


        engine.DrawLine({ 0.2, 0 }, { 0.5, 0 }, { 1, 0, 0, 1 });
        engine.DrawLine({ 0, 0.2 }, { 0, 0.5 }, { 0, 1, 0, 1 });

        engine.DrawLine({ 20, 20 }, { 50, 50 }, { 1, 0, 0, 1 });
        engine.DrawLine({ -20, -20 }, { -50, -50 }, { 0, 1, 0, 1 });

        engine.DrawLine({ 20, -20 }, { 50, -50 }, { 1, 0, 0, 1 });
        engine.DrawLine({ -20, 20 }, { -50, 50 }, { 0, 1, 0, 1 });

        engine.RenderFrame();
        engine.FlushFrame();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / (int)FPS_CAP));
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