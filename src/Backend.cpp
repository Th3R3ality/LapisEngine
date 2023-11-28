#include "Backend.h"

#include <iostream>
#include <chrono>
#include <DirectXMath.h>

#include "Helpers.h"

namespace Lapis
{

    void LapisInstance::Init()
    {
        this->initDuration = std::chrono::high_resolution_clock::now().time_since_epoch();
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

        /*
        // Set the viewport
        D3D11_VIEWPORT viewport;
        ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.Width = SCREEN_WIDTH;
        viewport.Height = SCREEN_HEIGHT;
        this->deviceContext->RSSetViewports(1, &viewport);
        */

        // Setup the viewport
        D3D11_VIEWPORT vp;
        vp.Width = (FLOAT)SCREEN_WIDTH;
        vp.Height = (FLOAT)SCREEN_HEIGHT;
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        vp.TopLeftX = 0;
        vp.TopLeftY = 0;
        this->deviceContext->RSSetViewports(1, &vp);



        InitPipeline();
    }

    void LapisInstance::InitPipeline()
    {
        // load and compile the two shaders
        ID3DBlob* VS, * PS;
        HRESULT hr1 = D3DCompileFromFile(L"src/shaders/unlit.shader", 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VShader", "vs_5_0", 0, 0, &VS, 0);
        HRESULT hr2 = D3DCompileFromFile(L"src/shaders/unlit.shader", 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PShader", "ps_5_0", 0, 0, &PS, 0);

        // encapsulate both shaders into shader objects
        this->device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &this->pVS_unlit);
        this->device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &this->pPS_unlit);

        // set the shader objects
        this->deviceContext->VSSetShader(this->pVS_unlit, 0, 0);
        this->deviceContext->PSSetShader(this->pPS_unlit, 0, 0);

        D3D11_INPUT_ELEMENT_DESC ied[] =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        };

        this->device->CreateInputLayout(ied, 3, VS->GetBufferPointer(), VS->GetBufferSize(), &this->pLayout);
        this->deviceContext->IASetInputLayout(this->pLayout);


        D3D11_BUFFER_DESC cbDesc{};
        cbDesc.ByteWidth = sizeof(GlobalConstantBuffer);
        cbDesc.Usage = D3D11_USAGE_DYNAMIC;
        cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        cbDesc.MiscFlags = 0;
        cbDesc.StructureByteStride = 0;

        this->device->CreateBuffer(&cbDesc, NULL, &this->pConstantBuffer);
        this->deviceContext->VSSetConstantBuffers(0, 1, &this->pConstantBuffer);
        this->deviceContext->PSSetConstantBuffers(0, 1, &this->pConstantBuffer);

        D3D11_RASTERIZER_DESC pRasterizerDesc = {};
        pRasterizerDesc.FillMode = D3D11_FILL_SOLID;
        pRasterizerDesc.CullMode = D3D11_CULL_NONE;

        ID3D11RasterizerState* pRasterizerState;

        this->device->CreateRasterizerState(&pRasterizerDesc, &pRasterizerState);

        this->deviceContext->RSSetState(pRasterizerState);
    }

    void LapisInstance::NewFrame() {
        static auto old = initDuration;

        auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
        this->deltaDuration = (now - old);
        this->elapsedDuration = (now - initDuration);
        old = now;

        this->deltaTime = (float)std::chrono::duration_cast<std::chrono::microseconds>(this->deltaDuration).count() / 1000 / 1000;
        this->elapsedTime += this->deltaTime;
    }

    void LapisInstance::CleanFrame() {
        
        this->vertexBuffer2D.clear();
        this->VerticeCount2D = 0;

        this->commandList2D.clear();



        this->vertexBuffer3D.clear();
        this->VerticeCount3D = 0;

        this->commandList3D.clear();
    }

    void LapisInstance::RenderFrame()
    {
        static GlobalConstantBuffer gcb;
        gcb.fTime = this->elapsedTime;

        float L = 0;
        float T = 0;
        float R = SCREEN_WIDTH;
        float B = SCREEN_HEIGHT;
        const float mvp[4][4] = {
            { 2.0f / (R - L),   0.0f,           0.0f,       0.0f },
            { 0.0f,             2.0f / (T - B),     0.0f,       0.0f },
            { 0.0f,             0.0f,           0.5f,       0.0f },
            { (R + L) / (L - R),(T + B) / (B - T),    0.5f,       1.0f },
        };

        auto screen = DirectX::XMMATRIX((const float*)mvp);

        

        auto world = DirectX::XMMatrixIdentity();


        DirectX::XMVECTOR Eye = Lapis::Helpers::XMVectorSet(Lapis::Vector4( 0.0f, 0.0f, 0.0f, 0.0f ));
        DirectX::XMVECTOR At = Lapis::Helpers::XMVectorSet(Lapis::Vector4(0.0f, 0.0f, 1.0f, 0.0f));
        DirectX::XMVECTOR Up = Lapis::Helpers::XMVectorSet(Lapis::Vector4(0.0f, 1.0f, 0.0f, 0.0f));
        
        auto view = DirectX::XMMatrixLookAtLH(Eye, At, Up);
        auto rotateView = DirectX::XMMatrixRotationY(this->CameraRotationY);
        auto translateView = DirectX::XMMatrixTranslation(this->cameraPosition.x, this->cameraPosition.y, this->cameraPosition.z);

        //view = view * rotateView * translateView;
        view = view * translateView;




        auto projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, 0.01f, 100.0f);


        

        
        static int VBufferSize = 0;
        if (this->VBufferCapacity3D > VBufferSize) {

            if (this->pVBuffer)
                this->pVBuffer->Release();

            std::cout << "resizing back buffer\n";
            // create the vertex buffer
            D3D11_BUFFER_DESC bd;
            ZeroMemory(&bd, sizeof(bd));

            bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
            bd.ByteWidth = sizeof(VERTEX) * this->VBufferCapacity3D;             // size is the VERTEX struct * 3
            bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
            bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer


            this->device->CreateBuffer(&bd, NULL, &this->pVBuffer);       // create the buffer
            if (!this->pVBuffer)
                return;

            VBufferSize = this->VBufferCapacity3D;
        }
        
        
        {
            D3D11_MAPPED_SUBRESOURCE ms;
            this->deviceContext->Map(this->pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
            memcpy(ms.pData, this->vertexBuffer3D.data(), this->vertexBuffer3D.size() * sizeof(VERTEX));
            this->deviceContext->Unmap(this->pVBuffer, NULL);
        }
        
        static float h = 0;
        h += this->deltaTime*0.5;
        if (h > 360) h -= 360;
        auto color = HSLToRGB((int)h, 1.0f, 0.7f, 1.0f);
        this->deviceContext->ClearRenderTargetView(this->backbuffer, (FLOAT*)&color);


        UINT stride = sizeof(VERTEX);
        UINT offset = 0;
        this->deviceContext->IASetVertexBuffers(0, 1, &this->pVBuffer, &stride, &offset);
        
        
        for (int i = 0; i < this->commandList3D.size(); i++) {
            {

                auto model = DirectX::XMMatrixIdentity();
                auto scaleModel = Lapis::Helpers::XMMatrixScaling(this->transformList.at(i).scale);
                auto rotateModel = Lapis::Helpers::XMMatrixRotationAxis(Lapis::Vector3(0,1,0),this->elapsedTime);
                auto translateModel = Lapis::Helpers::XMMatrixTranslation(this->transformList.at(i).pos);
                
                model = model * scaleModel * rotateModel * translateModel;

                gcb.Screen = screen;
                gcb.Model = DirectX::XMMatrixTranspose(model);
                gcb.World = DirectX::XMMatrixTranspose(world);
                gcb.View = DirectX::XMMatrixTranspose(view);
                gcb.Projection = DirectX::XMMatrixTranspose(projection);
                //this->deviceContext->UpdateSubresource(this->pConstantBuffer, 0, nullptr, &gcb, 0, 0);

                D3D11_MAPPED_SUBRESOURCE ms;
                this->deviceContext->Map(this->pConstantBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
                memcpy(ms.pData, &gcb, sizeof(gcb));
                this->deviceContext->Unmap(this->pConstantBuffer, NULL);
            }

            this->deviceContext->IASetPrimitiveTopology(this->commandList3D.at(i).TopologyType);
            this->deviceContext->Draw(this->commandList3D.at(i).VertexCount, this->commandList3D.at(i).StartVertexLocation);
        }

        this->swapchain->Present(0, 0);
    }

    void LapisInstance::PushCommand(int VerticeCount, D3D_PRIMITIVE_TOPOLOGY Topology, bool _3D) {
        if (!_3D) {
            this->commandList2D.push_back(LapisCommand(VerticeCount, this->VerticeCount2D, Topology));
        }
        else {
            this->commandList3D.push_back(LapisCommand(VerticeCount, this->VerticeCount3D, Topology));
        }
    }

    void LapisInstance::PushTransform(Transform transform) {
        this->transformList.push_back(transform);
    }


    void LapisInstance::PushVertex(float x, float y, DXGI_RGBA col, DirectX::XMFLOAT4 uv) {
        if (this->VerticeCount2D + 1 > this->VBufferCapacity2D) {
            this->VBufferCapacity2D += 1000;
            this->vertexBuffer2D.reserve(this->VBufferCapacity2D);
        }
        this->vertexBuffer2D.push_back(VERTEX({ x, y, 0 }, col, uv));
        this->VerticeCount2D += 1;
    }

    void LapisInstance::PushVertex(float x, float y, float z, DXGI_RGBA col, DirectX::XMFLOAT4 uv)
    {
        if (this->VerticeCount3D + 1 > this->VBufferCapacity3D) {
            this->VBufferCapacity3D += 1000;
            this->vertexBuffer3D.reserve(this->VBufferCapacity3D);
        }
        this->vertexBuffer3D.push_back(VERTEX({ x, y, z }, col, uv));
        this->VerticeCount3D += 1;
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
    bool LapisInstance::GetInput(int key)
    {
        return (GetAsyncKeyState(key) & 0x8000) != 0;

    }
}

