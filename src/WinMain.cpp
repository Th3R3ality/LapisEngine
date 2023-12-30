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
#include "engine/LapisEngine.h"
#include "engine/GlobalDefines.h"
#include "engine/Helpers.h"

// include Utility headers
#include "utils/hsl-to-rgb.hpp"


// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// entry point
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
    
    // define window class
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    //wc.style = ACS_TRANSPARENT;
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
    Lapis::InitLapis(hwnd);


    float FPS_CAP = 60;
    bool LIMIT_FPS = false;
    MSG msg{};
    while (true && !GetAsyncKeyState(VK_DELETE))
    {
        using namespace Lapis;
        using namespace Lapis::Draw;

        static float fpsTimeTotal = 0;
        static int fpsSamples = 0;
        static float fps = 0;
        if (GetAsyncKeyState('M')) {
            fpsTimeTotal += Lapis::deltaTime;
            fpsSamples++;
        }
        if (fpsSamples > 0) {
            fps = 1000/(1000 * fpsTimeTotal / fpsSamples);
        }
        std::cout << std::format("delta:   {:.8f}s\nelapsed: {:.4f}s\navg fps: {}\n", Lapis::deltaTime, Lapis::elapsedTime, fps);

        if (GetAsyncKeyState('A')) mainCamera.pos += Vec3::right * deltaTime;
        if (GetAsyncKeyState('D')) mainCamera.pos -= Vec3::right * deltaTime;
        if (GetAsyncKeyState('Q')) mainCamera.pos += Vec3::up * deltaTime;
        if (GetAsyncKeyState('E')) mainCamera.pos -= Vec3::up * deltaTime;
        if (GetAsyncKeyState('W')) mainCamera.pos -= Vec3::forward * deltaTime;
        if (GetAsyncKeyState('S')) mainCamera.pos += Vec3::forward * deltaTime;

        if (GetAsyncKeyState(VK_RIGHT)) mainCamera.rot += Vec3(0, 45, 0) * deltaTime;
        if (GetAsyncKeyState(VK_LEFT))  mainCamera.rot -= Vec3(0, 45, 0) * deltaTime;
        if (GetAsyncKeyState(VK_UP))    mainCamera.rot -= Vec3(45, 0, 0) * deltaTime;
        if (GetAsyncKeyState(VK_DOWN))  mainCamera.rot += Vec3(45, 0, 0) * deltaTime;

        Lapis::NewFrame();

        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
                break;
        }

        //D2::Line(Vec2(10, 10), Vec2(50, 50), {1,1,1,1});
        //D2::Rect(Vec4(100,10,110,60), {1,0,0,1});
        //D2::Rect(Vec2(10,100), Vec2(50,10), {0,0,1,1});
        //D2::Circle(Vec2(120), 20, { 0,1,0,1 });
        //D2::Circle(Vec2(150), 20, { 0,0,0,1 }, 12);
       
        D3::Plane(Transform(Vec3::forward + -Vec3::up * 0.3f, 0, 1), {1,1,1,1});
        D3::Cube(Transform(Vec3::forward * 3, 0, 1 ), { 0.92, 0.26, .27, 1});
        D3::Line(Vec3::forward * 3 + -Vec3::right, Vec3::forward * 2 + -Vec3::right * 1.5, { 0.345, 0.396, 0.949, 1});
        D3::Arrow(Vec3::forward * 1.5, Vec3(.1), { 0.996, 0.906, 0.361, 1 });

        Lapis::RenderFrame();
        Lapis::FlushFrame();
    }

    std::cout << "Cleaning up";
    Lapis::CleanLapis();

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