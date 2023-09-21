#include "Backend.h"

namespace Lapis
{
    void LapisInstance::Init()
    {

    }

    void LapisInstance::InitD3D11(HWND hwnd)
    {

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
            &this->swapchain,
            &this->device,
            NULL,
            &this->deviceContext);

        // get the address of the back buffer
        ID3D11Texture2D* pBackBuffer = nullptr;
        this->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

        // use the back buffer address to create the render target
        this->device->CreateRenderTargetView(pBackBuffer, NULL, &this->backbuffer);
        pBackBuffer->Release();

        // set the render target as the back buffer
        this->deviceContext->OMSetRenderTargets(1, &this->backbuffer, NULL);

        // Set the viewport
        D3D11_VIEWPORT viewport;
        ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.Width = SCREEN_WIDTH;
        viewport.Height = SCREEN_HEIGHT;

        this->deviceContext->RSSetViewports(1, &viewport);

        InitPipeline();
        InitGraphics();
    }

    void LapisInstance::CleanD3D11()
    {
        this->swapchain->SetFullscreenState(0, NULL);

        this->pLayout->Release();
        this->pVS_unlit->Release();
        this->pPS_unlit->Release();
        this->pVBuffer->Release();
        this->swapchain->Release();
        this->backbuffer->Release();
        this->device->Release();
        this->deviceContext->Release();
    }

    void LapisInstance::InitGraphics()
    {
        // create a triangle using the VERTEX struct
        VERTEX tri1[] =
        {
            {-0.9f, 0.85f, 0.0f, DXGI_RGBA(0.2f, 0.2f, 1.0f, 1.0f)},
            {0.9f, -0.90, 0.0f, DXGI_RGBA(0.8f, 0.8f, 0.8f, 1.0f)},
            {-0.9f, -0.90f, 0.0f, DXGI_RGBA(0.1f, 0.1f, 0.1f, 1.0f)}
        };
        CommandList.push_back({ 3, 0, 0, D3D10_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST });

        VERTEX tri2[] = {
            {-0.9f, 0.9f, 0.0f, DXGI_RGBA(1.0f, 0.0f, 0.0f, 1.0f)},
            {0.9f, 0.9f, 0.0f, DXGI_RGBA(0.0f, 1.0f, 0.0f, 1.0f)},
            {0.9f, -0.85f, 0.0f, DXGI_RGBA(0.0f, 0.0f, 1.0f, 1.0f)}
        };

        // create the vertex buffer
        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));

        bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
        bd.ByteWidth = sizeof(VERTEX) * 6;             // size is the VERTEX struct * 3
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

        this->device->CreateBuffer(&bd, NULL, &this->pVBuffer);       // create the buffer

        char combined_buffer[sizeof(tri1) + sizeof(tri2)];
        ZeroMemory(combined_buffer, sizeof(combined_buffer));

        memcpy(combined_buffer, tri1, sizeof(tri1));
        memcpy(combined_buffer + sizeof(tri1), tri2, sizeof(tri2));

        // copy the vertices into the buffer
        D3D11_MAPPED_SUBRESOURCE ms;
        this->deviceContext->Map(this->pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
        memcpy(ms.pData, combined_buffer, sizeof(combined_buffer) / sizeof(combined_buffer[0]));                 // copy the data
        this->deviceContext->Unmap(this->pVBuffer, NULL);                                  // unmap the buffer
    }

    void LapisInstance::InitPipeline()
    {
        // load and compile the two shaders
        ID3DBlob* VS, * PS;
        D3DCompileFromFile(L"src/unlit.shader", 0, 0, "VShader", "vs_4_0", 0, 0, &VS, 0);
        D3DCompileFromFile(L"src/unlit.shader", 0, 0, "PShader", "ps_4_0", 0, 0, &PS, 0);

        // encapsulate both shaders into shader objects
        this->device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &this->pVS_unlit);
        this->device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &this->pPS_unlit);

        // set the shader objects
        this->deviceContext->VSSetShader(this->pVS_unlit, 0, 0);
        this->deviceContext->PSSetShader(this->pPS_unlit, 0, 0);

        D3D11_INPUT_ELEMENT_DESC ied[] =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
        };

        this->device->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &this->pLayout);
        this->deviceContext->IASetInputLayout(this->pLayout);

    }

    // this is the function used to render a single frame
    void LapisInstance::RenderFrame()
    {
        /*
        // create the vertex buffer
        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));

        bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
        bd.ByteWidth = sizeof(VERTEX) * 6;             // size is the VERTEX struct * 3
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

        this->device->CreateBuffer(&bd, NULL, &this->pVBuffer);       // create the buffer
        if (!this->pVBuffer)
            return;

        char combined_buffer[64];
        ZeroMemory(combined_buffer, sizeof(combined_buffer));

        // copy the vertices into the buffer
        D3D11_MAPPED_SUBRESOURCE ms;
        this->deviceContext->Map(this->pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
        memcpy(ms.pData, combined_buffer, sizeof(combined_buffer) / sizeof(combined_buffer[0]));                 // copy the data
        this->deviceContext->Unmap(this->pVBuffer, NULL);
        */
        static int h = 0;
        static int timeout = 0;
        timeout += 1;
        if (timeout % 100 == 0) {
            h += 1; if (h > 360) h -= 360;
        }

        // clear the back buffer to a deep blue
        auto color = HSLToRGB(h, 0.7f, 0.7f, 1.0f);
        this->deviceContext->ClearRenderTargetView(this->backbuffer, (FLOAT*)&color);

        // do 3D rendering on the back buffer here

        // select which vertex buffer to display
        UINT stride = sizeof(VERTEX);
        UINT offset = 0;
        this->deviceContext->IASetVertexBuffers(0, 1, &this->pVBuffer, &stride, &offset);

        /*
        for (auto& command : this->CommandList) {
            // select which primtive type we are using
            this->deviceContext->IASetPrimitiveTopology(command.PrimitiveType);

            // draw the vertex buffer to the back buffer
            this->deviceContext->DrawIndexed(command.IndexCount, command.StartIndexLocation, command.BaseVertexLocation);
        }
        */
        this->deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        this->deviceContext->Draw(6, 0);

        // switch the back buffer and the front buffer
        this->swapchain->Present(0, 0);
    }
}

