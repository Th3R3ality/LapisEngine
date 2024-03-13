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
#include "Lapis/Engine.h"
#include "Lapis/Helpers.h"

// include Utility headers
#include "utils/hsl-to-rgb.hpp"


// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// entry point
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
    
    SetProcessDPIAware();

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

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    RECT wr = { 0, 0, screenWidth, screenWidth };
    HWND hwnd;

    

    // create the window and use the result as the handle
    hwnd = CreateWindowExW(
        NULL, //( WS_EX_TOPMOST | WS_EX_NOACTIVATE),
        wc.lpszClassName,    // name of the window class
        L"Lapis Dev Window",   // title of the window
        WS_POPUP,    // window style //
        wr.left,    // x-position of the window
        wr.top,    // y-position of the window
        wr.right -wr.left,    // width of the window
        wr.bottom -wr.top,    // height of the window
        NULL,    // we have no parent window, NULL
        NULL,    // we aren't using menus, NULL
        hInstance,    // application handle
        NULL);    // used with multiple windows, NULL
    ShowWindow(hwnd, nCmdShow); // make sure window is shown

    //SetLayeredWindowAttributes(hwnd, 0, 1.0f, LWA_ALPHA);
    //SetLayeredWindowAttributes(hwnd, 0, RGB(0, 0, 0), LWA_COLORKEY);
    //MARGINS margins = { -1, -1, -1, -1 }; ;
    //DwmExtendFrameIntoClientArea(hwnd, &margins);

    std::cout << "created device and swapchain\n";


#ifdef _DEBUG
    AllocConsole();
    SetConsoleTitleW(L"DEBUG OUTPUT");
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    printf("yoo!\n");
#endif

    printf("initting lapis\n");
    Lapis::InitLapis(hwnd);


    Lapis::Vec2 pos1(0), pos2(10);

    float FPS_CAP = 60;
    bool LIMIT_FPS = false;

    int maxLightness = 10;
    int xGrid = 4, yGrid = 4;
    bool running = false;
    bool holdingEdit = false;
    bool wasHoldingEdit = false;
    MSG msg{};
    while (true && !GetAsyncKeyState(VK_DELETE))
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
                break;
        }

        // Run Lapis Frame
        {
            using namespace Lapis;
            NewFrame();

            if (GetAsyncKeyState(VK_F4) & 1)
            {
                running = !running;
            }

            if (GetAsyncKeyState(VK_F2))
            {
                holdingEdit = true;
            }
            else
                holdingEdit = false;

            if (GetAsyncKeyState(VK_UP) & 1)
                yGrid += 1;
            if (GetAsyncKeyState(VK_DOWN) & 1)
                yGrid -= 1;
            if (GetAsyncKeyState(VK_LEFT) & 1)
                xGrid -= 1;
            if (GetAsyncKeyState(VK_RIGHT) & 1)
                xGrid += 1;

            if (xGrid <= 0) xGrid = 1;
            if (yGrid <= 0) yGrid = 1;

            if (!wasHoldingEdit && holdingEdit)
            {
                POINT p = {};
                GetCursorPos(&p);
                pos1 = Lapis::Vec2(p.x, p.y);
            }
            if (wasHoldingEdit)
            {
                POINT p = {};
                GetCursorPos(&p);
                pos2 = Lapis::Vec2(p.x, p.y);
            }

            Draw::D2::Rect(Lapis::Vec4(24,24,250,200), "101010");

            int xCenterOffset = (pos2.x - pos1.x) / xGrid / 2;
            int yCenterOffset = (pos2.y - pos1.y) / yGrid / 2;

            if (running)
            {

                SetWindowLongA(hwnd, GWL_EXSTYLE, (WS_EX_TOPMOST | WS_EX_NOACTIVATE));
                SetWindowPos(hwnd, NULL, 24, 24, 250, 200, SWP_SHOWWINDOW | SWP_NOMOVE);
                
                Draw::D2::String("running", { 48,48 }, "66ff66", 6);
                HDC hdc = GetDC(HWND_DESKTOP);
                for (int i = 0; i < xGrid; i++)
                {
                    auto xTemp = pos1.x + i * ((pos2.x - pos1.x) / xGrid) + xCenterOffset;
                    for (int j = 0; j < yGrid; j++)
                    {
                        auto yTemp = pos1.y + j * ((pos2.y - pos1.y) / yGrid) + yCenterOffset;
                        static COLORREF pixel = GetPixel(hdc, xTemp, yTemp);
                        BYTE r = GetRValue(pixel);
                        BYTE g = GetGValue(pixel);
                        BYTE b = GetBValue(pixel);

                        std::cout << std::format("{} {}\n", xTemp, yTemp);
                        std::cout << std::format("{} {} {}\n\n", r, g, b);
                        SetCursorPos(xTemp, yTemp);
                        std::this_thread::sleep_for(std::chrono::milliseconds(100));
                        if (r <= maxLightness && g <= maxLightness && b <= maxLightness)
                        {
                            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
                        }
                    }
                }
                ReleaseDC(0, hdc);
            }
            else
            {
                SetWindowLongA(hwnd, GWL_EXSTYLE, (WS_EX_TOPMOST | WS_EX_NOACTIVATE | WS_EX_LAYERED | WS_EX_TRANSPARENT));
                SetWindowPos(hwnd, NULL, 0, 0, screenWidth, screenHeight, SWP_SHOWWINDOW | SWP_NOMOVE);

                Draw::D2::String("stopped", { 48,48 }, "ff6666", 6);
                Draw::D2::Rect(pos1, pos2 - pos1, "00000050");
                for (int i = 0; i < xGrid; i++)
                {
                    auto xTemp = pos1.x + i * ((pos2.x - pos1.x) / xGrid) + xCenterOffset;
                    //Draw::D2::Line(Lapis::Vec2(xTemp, pos1.y), Lapis::Vec2(xTemp, pos2.y), "aa0000");

                    for (int j = 0; j < yGrid; j++)
                    {
                        auto yTemp = pos1.y + j * ((pos2.y - pos1.y) / yGrid) + yCenterOffset;
                        Draw::D2::Line(Lapis::Vec2(xTemp-10, yTemp), Lapis::Vec2(xTemp+10, yTemp), "5555ff");
                        Draw::D2::Line(Lapis::Vec2(xTemp, yTemp-10), Lapis::Vec2(xTemp, yTemp+10), "5555ff");
                    }
                }
                for (int i = 0; i < xGrid+1; i++)
                {
                    auto xTemp = pos1.x + i * ((pos2.x - pos1.x) / xGrid);
                    Draw::D2::Line(Lapis::Vec2(xTemp, pos1.y), Lapis::Vec2(xTemp, pos2.y), "444444");
                }
                for (int j = 0; j < yGrid+1; j++)
                {
                    auto yTemp = pos1.y + j * ((pos2.y - pos1.y) / yGrid);
                    Draw::D2::Line(Lapis::Vec2(pos1.x, yTemp), Lapis::Vec2(pos2.x, yTemp), "444444");
                }
            }

            Draw::D2::String("grid", { 48,48 * 2 }, "ffffff", 6);
            Draw::D2::String(std::format("x {}",xGrid).c_str(), { 48,48 * 2.75 }, "0050ff", 6);
            Draw::D2::String(std::format("y {}",yGrid).c_str(), { 48,48 * 3.5 }, "00ff50", 6);

            Draw::D3::Plane(transform, "ffffff90");
            //Draw::D3::Cube(transform, "ffffff90");
            Draw::D3::Arrow(transform.pos, transform.Forward(), "0000ff");
            Draw::D3::Arrow(transform.pos, transform.Right(), "ff0000");
            Draw::D3::Arrow(transform.pos, transform.Up(), "00ff00");

            Draw::D2::String("a bcdefghi", {48,48}, "ffffff", 12);

            wasHoldingEdit = holdingEdit;
            RenderFrame();
            FlushFrame();
        }
    }

    std::cout << "Cleaning up";
    Lapis::CleanLapis();

    return (int)msg.wParam;
}

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    Lapis::WndProcHandler(hwnd, msg, wParam, lParam);

    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
        break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}