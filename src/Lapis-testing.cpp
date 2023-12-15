// Lapis-testing.cpp : This file contains the 'main' function. Program execution begins and ends there.

// include the basic windows header files
#include <windows.h>
#include <windowsx.h>

// include chrono for time
#include <chrono>

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

    engine.Init();
    engine.InitD3D11(hwnd);

    

    MSG msg;
    while (true)
    {
        engine.NewFrame();
        std::cout << "delta: " << engine.deltaTime * 100 << "ms\t";

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


        static int checkerboardSize = 20;
        if (GetAsyncKeyState(VK_UP)) checkerboardSize += 1;
        if (GetAsyncKeyState(VK_DOWN)) checkerboardSize -= 1;

        std::cout << "Checkerboard Size: " << checkerboardSize << std::endl;

        for (int i = 0; i < checkerboardSize; i++) {
            for (int j = 0; j < checkerboardSize; j++) {
                //float dist = ((DirectX::XMVector2Length({ (float)i - checkerboardSize / 2,(float)j - checkerboardSize / 2 })).m128_f32[0]) / (checkerboardSize / 2);
                DXGI_RGBA col;
                if (((i % 2) + j) % 2 == 1)
                    col = { 1 ,1 ,1 ,1 };
                else
                    col = { 0, 0 ,0 ,1 };
                //col = { dist,dist,dist,1 };
                //engine.DrawPlane(Transform({ i - checkerboardSize / 2,-2 - dist * dist * (checkerboardSize / 5), j - checkerboardSize / 2 }, {}, { 1,1,1 }), col);
                engine.DrawPlane(Transform({ i - checkerboardSize / 2,-2, j - checkerboardSize / 2 }, {}, {1,1,1}), col);
            }
        }

        engine.DrawCube(Transform({2,1,2}, {0,0,0}, {1,1,1}), { 0, 1, 0, 1 });

        for (int i = 0; i < 32; i++) {
            float _x = cosf(i * DirectX::XM_PI / (10 - 1)) * 0.5 + 0.5;
            float _y = (i % 2 == 0 ? -1 : 1) * sinf(i * DirectX::XM_PI / (10 - 1)) * 0.5 + 0.5;
            engine.DrawTriangle3D(
                {{_x,-1,_y},{0.0,0.0,0.0},{0.1,0.1,0.1}},
                { _x,0.0,_y,1.0 });
            engine.DrawTriangle3D(
                {{_x,-1.5f + _y,0.5},{0.0,0.0,0.0},{0.1,0.1,0.1}},
                {_x, _y,0.0,1.0} );
            engine.DrawTriangle3D(
                {{0.5,-1.5f + _y,_x},{0.0,0,0.0},{ 0.1,0.1,0.1 }},
                { 0.0,_y,_x,1.0 });
        }


        engine.DrawTriangle3D(
            Transform(
                Vector3(-2, 0, 0),
                Vector3(0, engine.elapsedTime*3, 0),
                Vector3(1, 1, 1)
            ), { 1.0, 0.8, 1.0, 1.0 });

        engine.DrawTriangle3D({ { 0, 0, 5 },{0,engine.elapsedTime,0}, {2,0.5,1} }, { 0.2, 1.0, 0.2, 1.0 });
        engine.DrawTriangle3D({ { 0, 2, 5 },{0,engine.elapsedTime,0}, {0.5,2,1} }, { 0.2, 0.2, 1.0, 1.0 });
        
        


        /* 2d

        static float x = 100;
        static float y = 75;
        static float wishDistance;
        static float moveDistance;

        wishDistance += engine.deltaTime * 100;

        while (wishDistance > 1.0f) {
            wishDistance -= 1;
            moveDistance += 1;
        }

        if (GetAsyncKeyState(VK_LEFT)) x -= moveDistance;
        if (GetAsyncKeyState(VK_RIGHT)) x += moveDistance;
        if (GetAsyncKeyState(VK_UP)) y -= moveDistance;
        if (GetAsyncKeyState(VK_DOWN)) y += moveDistance;
        std::cout << "x: " << x << " - y: " << y << "\n";
        
        moveDistance = 0;

        
        float t = sinf(engine.elapsedTime)*0.5 +0.5;

        static bool resizeCube = false;
        if (GetAsyncKeyState(VK_SPACE) & 0x0001) {
            resizeCube = !resizeCube;
        }

        if (!resizeCube) {
            t = 0.5f;
        }

        engine.DrawPoint(10, 10);
        engine.DrawRect(x - 50*t, y - 50*t, 100*t, 100*t);
        engine.DrawPoint(x + .5, y + .5);

        engine.DrawRect(10, 10, 100, 25);


        float lineLength = (sinf(engine.elapsedTime * 0.05)*0.5+0.5) * 65;
        engine.DrawLine(
            lineLength * sinf(engine.elapsedTime*.5) + 100,
            lineLength * cosf(engine.elapsedTime*.5) + 75,
            lineLength * -sinf(engine.elapsedTime*0.5) + 100,
            lineLength * -cosf(engine.elapsedTime*0.5) + 75);
        
        
        //engine.DrawRect(100, 75, 50, 50, {0.0,0.0,0.0,0.0});
        engine.DrawCircle(140, 90, 50, 50, { 1.0,1.0,1.0,1.0 }, (int)((cosf(engine.elapsedTime*0.1)*0.5+0.5)*64)+6);

        */

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