// Lapis-testing.cpp : This file contains the 'main' function. Program execution begins and ends there.

// include the basic windows header files
#include <windows.h>
#include <windowsx.h>
#include <wingdi.h>

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
#include "engine/Backend.h"
#include "engine/DataTypes.hpp"
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
    hwnd = CreateWindowExW(
        NULL,//( WS_EX_TOPMOST | WS_EX_NOACTIVATE),
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



        Vec2 center = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};

        //engine.DrawLine(center + Vec2(20, 0), center + Vec2(50, 0), { 1, 0, 0, 1 });
        //engine.DrawLine(center + Vec2(0, 20), center + Vec2(0, 50), { 0, 1, 0, 1 });
        //engine.DrawRect(center - Vec2(10, 10), Vec2(20, 20), { 0,0,1,1 });
        //engine.DrawRect(Vec4(10, 10, 190, 15), { 1,0,1,1 });
        //engine.DrawCircle(Vec2(10, 50), 10, { 1,1,0,1 }, 16);

        engine.DrawPlane(Transform(Vec3(0, -0.1, 0), {}, { 1 }), { 0.5,0.5,0.5,1 });

        int checkerboardSize = 10;
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
                engine.DrawPlane(Transform(Vec3(i - checkerboardSize / 2 + 0.5, -0.2, j - checkerboardSize / 2 + 0.5), {}, {1}), col);
            }
        }

        engine.DrawLine3D(Vec3(0, 0, 1), Vec3(1, 0, 0), { 0.8,0.2,0.8,1 });

        float ymod = 0.3*sinf(engine.elapsedTime) + 0.4;

        std::vector<Vec3> lineSegments = {
            Vec3(0   ,.1 * ymod,1),
            Vec3(.7  ,.2 * ymod,.7),
            Vec3(1   ,.3 * ymod,0),
            Vec3(0.7 ,.4 * ymod,-0.7),
            Vec3(0   ,.5 * ymod,-1),
            Vec3(-0.7,.6 * ymod,-0.7),
            Vec3(-1  ,.7 * ymod,0),
            Vec3(-0.7,.8 * ymod,0.7),

            Vec3(0   ,.9 * ymod,1),
            Vec3(.7  ,1 * ymod,.7),
            Vec3(1   ,1.1 * ymod,0),
            Vec3(0.7 ,1.2 * ymod,-0.7),
            Vec3(0   ,1.3 * ymod,-1),
            Vec3(-0.7,1.4 * ymod,-0.7),
            Vec3(-1  ,1.5 * ymod,0),
            Vec3(-0.7,1.6 * ymod,0.7),

            Vec3(0   ,1.7 * ymod,1),
            Vec3(.7  ,1.8 * ymod,.7),
            Vec3(1   ,1.9 * ymod,0),
            Vec3(0.7 ,2 * ymod,-0.7),
            Vec3(0   ,2.1 * ymod,-1),
            Vec3(-0.7,2.2 * ymod,-0.7),
            Vec3(-1  ,2.3 * ymod,0),
            Vec3(-0.7,2.4 * ymod,0.7),

            Vec3(0   ,2.5 * ymod,1),
            Vec3(.7  ,2.6 * ymod,.7),
            Vec3(1   ,2.7 * ymod,0),
            Vec3(0.7 ,2.8 * ymod,-0.7),
            Vec3(0   ,2.9 * ymod,-1),
            Vec3(-0.7,3 * ymod,-0.7),
            Vec3(-1  ,3.1 * ymod,0),
            Vec3(-0.7,3.2 * ymod,0.7),
            Vec3(0   ,3.3 * ymod,1),
        };
        engine.DrawLines3D(lineSegments, { 0.2,0.2,0.8,1 });

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