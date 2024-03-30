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

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_win32.h"
#include "imgui/backends/imgui_impl_dx11.h"


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
        NULL, //( WS_EX_TOPMOST | WS_EX_NOACTIVATE),
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

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImGui_ImplWin32_Init(hwnd);

    ID3D11Device* device;
    ID3D11DeviceContext* deviceContext;
    Lapis::GetDeviceAndCtx(&device, &deviceContext);
    ImGui_ImplDX11_Init(device, deviceContext);

    float FPS_CAP = 60;
    bool LIMIT_FPS = false;
    MSG msg{};
    while (true && !GetAsyncKeyState(VK_ESCAPE))
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

            ////std::cout << "mainCamera.pos : " << mainCamera.pos << "\n";
            ////std::cout << "mainCamera.rot : " << mainCamera.rot << "\n";

            if (GetAsyncKeyState('W')) mainCamera.pos += mainCamera.Forward() * deltaTime;
            if (GetAsyncKeyState('S')) mainCamera.pos -= mainCamera.Forward() * deltaTime;
            if (GetAsyncKeyState('A')) mainCamera.pos -= mainCamera.Right() * deltaTime;
            if (GetAsyncKeyState('D')) mainCamera.pos += mainCamera.Right() * deltaTime;
            if (GetAsyncKeyState('Q')) mainCamera.pos -= mainCamera.Up() * deltaTime;
            if (GetAsyncKeyState('E')) mainCamera.pos += mainCamera.Up() * deltaTime;

            if (GetAsyncKeyState(VK_RIGHT)) mainCamera.rot.yaw -= 90 * deltaTime;
            if (GetAsyncKeyState(VK_LEFT))  mainCamera.rot.yaw += 90 * deltaTime;
            if (GetAsyncKeyState(VK_DOWN)) mainCamera.rot.pitch -= 90 * deltaTime;
            if (GetAsyncKeyState(VK_UP))  mainCamera.rot.pitch += 90 * deltaTime;
            if (GetAsyncKeyState('Z')) mainCamera.rot.roll -= 90 * deltaTime;
            if (GetAsyncKeyState('X'))  mainCamera.rot.roll += 90 * deltaTime;


            Draw::D2::Circle(-5, 30, "00ff50");

            static int checkerboardSize = 25;
            Color col;
            for (int i = 0; i < checkerboardSize; i++) {
                for (int j = 0; j < checkerboardSize; j++) {
                    if (((i % 2) + j) % 2 == 1)
                        col = "707070";
                    else
                        col = "101010";
                    Draw::D3::Plane(Transform(Vec3(i - checkerboardSize / 2, -2, j - checkerboardSize / 2), 0, 1), col);
                }
            }
            
            static auto transform = Transform(Vec3(0,-0.5,2), 0, 0.1);

            if (GetAsyncKeyState(VK_NUMPAD4))
            {
                transform.rot.yaw += 20 * deltaTime;
            }
            if (GetAsyncKeyState(VK_NUMPAD6))
            {
                transform.rot.yaw -= 20 * deltaTime;
            }
            if (GetAsyncKeyState(VK_NUMPAD8))
            {
                transform.rot.pitch += 20 * deltaTime;
            }
            if (GetAsyncKeyState(VK_NUMPAD2))
            {
                transform.rot.pitch -= 20 * deltaTime;
            }
            if (GetAsyncKeyState(VK_NUMPAD7))
            {
                transform.rot.roll += 20 * deltaTime;
            }
            if (GetAsyncKeyState(VK_NUMPAD9))
            {
                transform.rot.roll -= 20 * deltaTime;
            }

            //transform.rot.pitch += 20 * deltaTime;

            Draw::D3::Plane(transform, "ffffff90");
            Draw::D3::Cube(transform, "ffffff90");
            Draw::D3::Arrow(transform.pos, transform.Forward(), "0000ff");
            Draw::D3::Arrow(transform.pos, transform.Right(), "ff0000");
            Draw::D3::Arrow(transform.pos, transform.Up(), "00ff00");

            Draw::D2::String("yo", { 48,48 }, "ffffff", 12);

            RenderFrame();
            FlushFrame();
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Hello, world!");
        ImGui::Text("Hello from another window!");
        ImGui::End();

        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        Lapis::PresentFrame();
    }

    std::cout << "Cleaning up";
    
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    
    Lapis::CleanLapis();

    return (int)msg.wParam;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    Lapis::WndProcHandler(hwnd, msg, wParam, lParam);
    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
        return true;

    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
        break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}