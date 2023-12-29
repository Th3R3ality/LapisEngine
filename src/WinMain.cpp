// Lapis-testing.cpp : This file contains the 'main' function. Program execution begins and ends there.

// include the basic windows header files
#include <windows.h>
#include <windowsx.h>
#include <wingdi.h>

#pragma comment(lib, "dwmapi.lib")
#include <dwmapi.h>

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
#include "engine/LapisEngine.h"
#include "engine/LapisTypes.h"

// include Utility headers
#include "utils/hsl-to-rgb.hpp"

//using namespace Lapis;
Lapis::LapisInstance engine;

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// entry point
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
    
    // define window class
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    //wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.style = ACS_TRANSPARENT;
    wc.lpfnWndProc = WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_NO);
    wc.hbrBackground;
    wc.lpszMenuName;
    wc.lpszClassName = L"LapisWindowClass";
    wc.hIconSm;

    // register the window class
    RegisterClassEx(&wc);

    RECT wr = { 0, 0, 800, 600 };
    HWND hwnd;

    // create the window and use the result as the handle
    hwnd = CreateWindowExW(
        NULL,//( WS_EX_TOPMOST | WS_EX_NOACTIVATE),
        wc.lpszClassName,    // name of the window class
        L"Lapis Dev Window",   // title of the window
        WS_OVERLAPPEDWINDOW,    // window style //WS_POPUP
        300,    // x-position of the window
        300,    // y-position of the window
        wr.right -wr.left,    // width of the window
        wr.bottom -wr.top,    // height of the window
        NULL,    // we have no parent window, NULL
        NULL,    // we aren't using menus, NULL
        hInstance,    // application handle
        NULL);    // used with multiple windows, NULL
    ShowWindow(hwnd, nCmdShow); // make sure window is shown

    //SetLayeredWindowAttributes(hwnd, 0, 1.0f, LWA_ALPHA);
    //SetLayeredWindowAttributes(hwnd, 0, RGB(0, 0, 0), LWA_COLORKEY);
    //MARGINS margins = { -1 }; ;
    //DwmExtendFrameIntoClientArea(hwnd, &margins);

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

    //Lapis::Init(Lapis::RenderApi::D3D11, hwnd);

    float FPS_CAP = 60;
    bool LIMIT_FPS = false;
    MSG msg;
    while (true)
    {

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



        Lapis::Vec2 center = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};

        //engine.DrawLine(center + Lapis::Vec2(20, 0), center + Lapis::Vec2(50, 0), { 1, 0, 0, 1 });
        //engine.DrawLine(center + Lapis::Vec2(0, 20), center + Lapis::Vec2(0, 50), { 0, 1, 0, 1 });

        engine.DrawPlane(Lapis::Transform(Lapis::Vec3(0, -0.1f, 0), {}, { 1 }), { 0.5,0.5,0.5,1 });

        int checkerboardSize = 10;
        for (int i = 0; i < checkerboardSize; i++) {
            for (int j = 0; j < checkerboardSize; j++) {
                DXGI_RGBA col;
                if (((i % 2) + j) % 2 == 1)
                    col = { 1 ,1 ,1 ,1 };
                else
                    col = { 0, 0 ,0 ,1 };
                engine.DrawPlane(Lapis::Transform(Lapis::Vec3(i - checkerboardSize / 2 + 0.5f, -0.2f, j - checkerboardSize / 2 + 0.5f - 5), {}, {1}), col);
            }
        }

        engine.DrawLine3D(Lapis::Vec3(0, 0, 1), Lapis::Vec3(1, 0, 0), { 0.8f,0.2f,0.8f,1 });

        float ymod = 0.3f*sinf(engine.elapsedTime) + 0.4f;

        std::vector<Lapis::Vec3> lineSegments = {
            Lapis::Vec3(0    ,.1f * ymod,1),
            Lapis::Vec3(.7f  ,.2f * ymod,.7f),
            Lapis::Vec3(1    ,.3f * ymod,0),
            Lapis::Vec3(0.7f ,.4f * ymod,-0.7f),
            Lapis::Vec3(0    ,.5f * ymod,-1),
            Lapis::Vec3(-0.7f,.6f * ymod,-0.7f),
            Lapis::Vec3(-1   ,.7f * ymod,0),
            Lapis::Vec3(-0.7f,.8f * ymod,0.7f),

            Lapis::Vec3(0    ,.9f * ymod,1),
            Lapis::Vec3(.7f  ,1 * ymod  ,.7f),
            Lapis::Vec3(1    ,1.1f * ymod,0),
            Lapis::Vec3(0.7f ,1.2f * ymod,-0.7f),
            Lapis::Vec3(0    ,1.3f * ymod,-1),
            Lapis::Vec3(-0.7f,1.4f * ymod,-0.7f),
            Lapis::Vec3(-1   ,1.5f * ymod,0),
            Lapis::Vec3(-0.7f,1.6f * ymod,0.7f),

            Lapis::Vec3(0    ,1.7f * ymod,1),
            Lapis::Vec3(.7f  ,1.8f * ymod,.7f),
            Lapis::Vec3(1    ,1.9f * ymod,0),
            Lapis::Vec3(0.7f ,2 * ymod,-0.7f),
            Lapis::Vec3(0    ,2.1f * ymod,-1),
            Lapis::Vec3(-0.7f,2.2f * ymod,-0.7f),
            Lapis::Vec3(-1   ,2.3f * ymod,0),
            Lapis::Vec3(-0.7f,2.4f * ymod,0.7f),

            Lapis::Vec3(0    ,2.5f * ymod,1),
            Lapis::Vec3(.7f  ,2.6f * ymod,.7f),
            Lapis::Vec3(1    ,2.7f * ymod,0),
            Lapis::Vec3(0.7f ,2.8f * ymod,-0.7f),
            Lapis::Vec3(0    ,2.9f * ymod,-1),
            Lapis::Vec3(-0.7f,3 * ymod,-0.7f),
            Lapis::Vec3(-1   ,3.1f * ymod,0),
            Lapis::Vec3(-0.7f,3.2f * ymod,0.7f),
            Lapis::Vec3(0    ,3.3f * ymod,1),
        };
        engine.DrawLines3D(lineSegments, { 0.5f,0.9f,0.5f,1 });

        //engine.DrawCircle(Lapis::Vec2(50, 50), 20, { 1,1,1,1 });
        //engine.DrawCircle(Lapis::Vec2(50, 48), 15, { 0,0,0,1 });
        //engine.DrawCircle(Lapis::Vec2(50, 46), 10, { 1,1,1,1 });

        //engine.DrawCircle(Lapis::Vec2(100, 50), 20, { 1,1,1,1 });
        //engine.DrawCircle(Lapis::Vec2(100, 48), 15, { 0,0,0,1 });
        //engine.DrawCircle(Lapis::Vec2(100, 46), 10, { 1,1,1,1 });

        //engine.DrawLine(Lapis::Vec2(40, 80), Lapis::Vec2(75, 100), {});
        //engine.DrawLine(Lapis::Vec2(40, 81), Lapis::Vec2(75, 101), {});
        //engine.DrawLine(Lapis::Vec2(40, 82), Lapis::Vec2(75, 102), {});

        //engine.DrawLine(Lapis::Vec2(75, 100), Lapis::Vec2(110, 80), {});
        //engine.DrawLine(Lapis::Vec2(75, 101), Lapis::Vec2(110, 81), {});
        //engine.DrawLine(Lapis::Vec2(75, 102), Lapis::Vec2(110, 82), {});

        engine.RenderFrame();
        engine.FlushFrame();
        if (LIMIT_FPS && FPS_CAP > 1)
            std::this_thread::sleep_for(std::chrono::milliseconds(1000 / (int)FPS_CAP));
    }

    std::cout << "Cleaning up";
    engine.CleanD3D11();

    return (int)msg.wParam;
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