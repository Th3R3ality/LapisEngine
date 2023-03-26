// Lapis-testing.cpp : This file contains the 'main' function. Program execution begins and ends there.

// include the basic windows header files and the Direct3D header files
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <chrono>

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

#include "Lapis-types.hpp"
#include "utils/hsl-to-rgb.hpp"

#define SCREEN_WIDTH 200
#define SCREEN_HEIGHT 150

// global declarations
IDXGISwapChain* g_swapchain; // the pointer to the swap chain interface
ID3D11Device* g_device; // the pointer to our Direct3D device interface
ID3D11DeviceContext* g_device_ctx; // the pointer to our Direct3D device context
ID3D11RenderTargetView* g_backbuffer;    // global declaration
ID3D11InputLayout* g_pLayout;
ID3D11VertexShader* g_pVS_unlit;    // the vertex shader
ID3D11PixelShader* g_pPS_unlit;     // the pixel shader
ID3D11Buffer* g_pVBuffer;

float g_deltaTime = 0;


// forward declarations
void initd3d(HWND hwnd);
void cleand3d();
void RenderFrame();
void InitPipeline();
void InitGraphics();

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
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    //wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = L"WindowClass1";

    // register the window class
    RegisterClassEx(&wc);

    RECT wr = { 0, 0, 800, 600 };
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, false);

    // create the window and use the result as the handle
    hwnd = CreateWindowEx(NULL,
        L"WindowClass1",    // name of the window class
        L"Our First Windowed Program",   // title of the window
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

    initd3d(hwnd);

    MSG msg;
    while (true)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
                break;
        }
        
        //
        // 
        // 
        // fix
        // 
        // 
        // 
        //g_deltaTime = std::chrono::system_clock::now().time_since_epoch().count();
        RenderFrame();
    }

    cleand3d();

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

void initd3d(HWND hwnd) {

    DXGI_SWAP_CHAIN_DESC scd;

    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

    scd.BufferCount = 1;                                    // one back buffer
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
    scd.OutputWindow = hwnd;                                // the window to be used
    scd.SampleDesc.Count = 4;                               // how many multisamples
    scd.Windowed = TRUE;                                    // windowed/full-screen mode
    scd.BufferDesc.Width = SCREEN_WIDTH;
    scd.BufferDesc.Height = SCREEN_HEIGHT;
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    // create a device, device context and swap chain using the information in the scd struct
    D3D11CreateDeviceAndSwapChain(NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        NULL,
        NULL,
        NULL,
        D3D11_SDK_VERSION,
        &scd,
        &g_swapchain,
        &g_device,
        NULL,
        &g_device_ctx);

    // get the address of the back buffer
    ID3D11Texture2D* pBackBuffer;
    g_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

    // use the back buffer address to create the render target
    g_device->CreateRenderTargetView(pBackBuffer, NULL, &g_backbuffer);
    pBackBuffer->Release();

    // set the render target as the back buffer
    g_device_ctx->OMSetRenderTargets(1, &g_backbuffer, NULL);

    // Set the viewport
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = SCREEN_WIDTH;
    viewport.Height = SCREEN_HEIGHT;

    g_device_ctx->RSSetViewports(1, &viewport);

    InitPipeline();
    InitGraphics();
}

// this is the function used to render a single frame
void RenderFrame()
{
    static int h = 0;
    //static int direction = 1;
    //if (h < 0 || h > 360) {
    //    direction *= -1;
    //}
    //h += 1 * direction;
    static int timeout = 0;
    timeout += 1;
    if (timeout % 100 == 0) {
        h += 1; if (h > 360) h -= 360;
    }

    // clear the back buffer to a deep blue
    g_device_ctx->ClearRenderTargetView(g_backbuffer, HSLToRGB(h, 0.7f, 0.7f, 1.0f));

    // do 3D rendering on the back buffer here

    // select which vertex buffer to display
    UINT stride = sizeof(VERTEX);
    UINT offset = 0;
    g_device_ctx->IASetVertexBuffers(0, 1, &g_pVBuffer, &stride, &offset);

    // select which primtive type we are using
    g_device_ctx->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // draw the vertex buffer to the back buffer
    g_device_ctx->Draw(6, 0);

    // switch the back buffer and the front buffer
    g_swapchain->Present(0, 0);
}

void InitPipeline()
{
    // load and compile the two shaders
    ID3D10Blob* VS, * PS;
    D3DX11CompileFromFile(L"src/unlit.shader", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, 0, 0);
    D3DX11CompileFromFile(L"src/unlit.shader", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, 0, 0);

    // encapsulate both shaders into shader objects
    g_device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &g_pVS_unlit);
    g_device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &g_pPS_unlit);

    // set the shader objects
    g_device_ctx->VSSetShader(g_pVS_unlit, 0, 0);
    g_device_ctx->PSSetShader(g_pPS_unlit, 0, 0);

    D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    g_device->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &g_pLayout);
    g_device_ctx->IASetInputLayout(g_pLayout);

}

void InitGraphics()
{
    // create a triangle using the VERTEX struct
    VERTEX tri1[] =
    {
        {-0.9f, 0.85f, 0.0f, D3DXCOLOR(0.2f, 0.2f, 1.0f, 1.0f)},
        {0.9f, -0.90, 0.0f, D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f)},
        {-0.9f, -0.90f, 0.0f, D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f)}
    };

    VERTEX tri2[] = {
        {-0.9f, 0.9f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)},
        {0.9f, 0.9f, 0.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)},
        {0.9f, -0.85f, 0.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f)}
    };

    // create the vertex buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
    bd.ByteWidth = sizeof(VERTEX) * 6;             // size is the VERTEX struct * 3
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

    g_device->CreateBuffer(&bd, NULL, &g_pVBuffer);       // create the buffer

    char combined_buffer[sizeof(tri1) + sizeof(tri2)];
    ZeroMemory(combined_buffer, sizeof(combined_buffer));

    memcpy(combined_buffer, tri1, sizeof(tri1));
    memcpy(combined_buffer + sizeof(tri1), tri2, sizeof(tri2));

    // copy the vertices into the buffer
    D3D11_MAPPED_SUBRESOURCE ms;
    g_device_ctx->Map(g_pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
    memcpy(ms.pData, combined_buffer, sizeof(combined_buffer)/sizeof(combined_buffer[0]));                 // copy the data
    g_device_ctx->Unmap(g_pVBuffer, NULL);                                  // unmap the buffer
}

void cleand3d()
{
    g_swapchain->SetFullscreenState(0, NULL);

    g_pLayout->Release();
    g_pVS_unlit->Release();
    g_pPS_unlit->Release();
    g_pVBuffer->Release();
    g_swapchain->Release();
    g_backbuffer->Release();
    g_device->Release();
    g_device_ctx->Release();
}