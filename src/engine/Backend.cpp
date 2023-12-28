#include "Backend.h"

#include <iostream>
#include <chrono>
#include <DirectXMath.h>

#include "shaders/DefaultShaders.h"

#include "Helpers.h"

namespace Lapis
{
    namespace Backend
    {
        void InitD3D11(HWND hwnd)
        {

        }
    }

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

        std::cout << "created device and swapchain\n";

        // get the address of the back buffer
        ID3D11Texture2D* pBackBuffer = nullptr;
        this->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

        // use the back buffer address to create the render target
        this->device->CreateRenderTargetView(pBackBuffer, NULL, &this->frameBuffer);
        pBackBuffer->Release();

        // set the render target as the back buffer
        this->deviceContext->OMSetRenderTargets(1, &this->frameBuffer, NULL);

        InitDefaultShaders();
        deviceContext->VSSetShader(builtinMaterials["UI"]->vertexShader, 0, 0);
        deviceContext->PSSetShader(builtinMaterials["UI"]->pixelShader, 0, 0);

        D3D11_INPUT_ELEMENT_DESC ied[] =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        };

        this->device->CreateInputLayout(ied, sizeof(ied) / sizeof(ied[0]), DEFAULTSHADER__UI_vertex, sizeof(DEFAULTSHADER__UI_vertex), &this->inputLayout);
        this->deviceContext->IASetInputLayout(this->inputLayout);


        D3D11_BUFFER_DESC cbDesc{};
        cbDesc.ByteWidth = sizeof(GlobalConstantBuffer);
        cbDesc.Usage = D3D11_USAGE_DYNAMIC;
        cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        cbDesc.MiscFlags = 0;
        cbDesc.StructureByteStride = 0;

        this->device->CreateBuffer(&cbDesc, NULL, &this->constantBuffer);
        this->deviceContext->VSSetConstantBuffers(0, 1, &this->constantBuffer);
        this->deviceContext->PSSetConstantBuffers(0, 1, &this->constantBuffer);

        // Setup the viewport
        D3D11_VIEWPORT vp;
        vp.Width = (FLOAT)SCREEN_WIDTH;
        vp.Height = (FLOAT)SCREEN_HEIGHT;
        vp.MinDepth = 0.0f; vp.TopLeftX = 0;
        vp.MaxDepth = 1.0f; vp.TopLeftY = 0;
        deviceContext->RSSetViewports(1, &vp);

        D3D11_RECT rect = {};
        rect.left = 0;
        rect.right = SCREEN_WIDTH;
        rect.top = 0;
        rect.bottom = SCREEN_HEIGHT;

        deviceContext->RSSetScissorRects(1, &rect);

        D3D11_RASTERIZER_DESC rasterizerDesc = {};
        rasterizerDesc.FillMode = D3D11_FILL_SOLID;
        rasterizerDesc.CullMode = D3D11_CULL_NONE;
        rasterizerDesc.ScissorEnable = true;
        rasterizerDesc.DepthClipEnable = true;


        ID3D11RasterizerState* pRasterizerState;
        this->device->CreateRasterizerState(&rasterizerDesc, &pRasterizerState);
        this->deviceContext->RSSetState(pRasterizerState);


        D3D11_TEXTURE2D_DESC depthBufferDesc;

        pBackBuffer->GetDesc(&depthBufferDesc); // copy from framebuffer properties

        depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

        ID3D11Texture2D* depthBuffer;

        device->CreateTexture2D(&depthBufferDesc, nullptr, &depthBuffer);


        device->CreateDepthStencilView(depthBuffer, nullptr, &this->depthBufferView);

        deviceContext->OMSetRenderTargets(1, &this->frameBuffer, this->depthBufferView);

        D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
        depthStencilDesc.DepthEnable = TRUE;
        depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

        ID3D11DepthStencilState* depthStencilState;

        device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);

        deviceContext->OMSetDepthStencilState(depthStencilState, 0);
        deviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff); // use default blend mode (i.e. disable)
    }
    void LapisInstance::CleanD3D11()
    {
        swapchain->SetFullscreenState(0, NULL);

        inputLayout->Release();

        for (auto [name, material] : builtinMaterials) {
            
        }
        builtinMaterials.clear();

        vertexBuffer->Release();
        frameBuffer->Release();

        swapchain->Release();
        device->Release();
        deviceContext->Release();
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
    void LapisInstance::RenderFrame()
    {
        
        static int VBufferSize = 0;
        if (VertexVectorCapacity > VBufferSize) {

            if (vertexBuffer)
                vertexBuffer->Release();

            std::cout << "resizing back buffer\n";
            D3D11_BUFFER_DESC bd = {};

            bd.Usage = D3D11_USAGE_DYNAMIC;
            bd.ByteWidth = VertexVectorCapacity * sizeof(Vertex);
            bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;


            this->device->CreateBuffer(&bd, NULL, &this->vertexBuffer);       // create the buffer
            if (!this->vertexBuffer)
                return;

            VBufferSize = this->VertexVectorCapacity;
        }
        
        RemapSubResource(vertexBuffer, LapisVertexVector.data(), sizeof(Vertex) * LapisVertexVector.size());
        
        static float h = 0;
        h += this->deltaTime*0.5f;
        if (h > 360) h -= 360;
        auto color = hsl2rgb((int)h, 1.0f, 0.7f, 1.0f);

        this->deviceContext->ClearRenderTargetView(this->frameBuffer, (FLOAT*)&color);
        this->deviceContext->ClearDepthStencilView(this->depthBufferView, D3D11_CLEAR_DEPTH, 1.0f, 0);

        UINT stride = sizeof(Vertex);
        UINT offset = 0;
        this->deviceContext->IASetVertexBuffers(0, 1, &this->vertexBuffer, &stride, &offset);
        
        UpdateGlobalConstantBuffer();

        for (auto& internalCommand : LapisCommandVector) {
            {
                this->DrawCommand(internalCommand);
            }
        }
        this->swapchain->Present(0, 0);
    }
    void LapisInstance::FlushFrame() {
        
        this->LapisVertexVector.clear();
        this->VertexCount = 0;

        this->LapisCommandVector.clear();
    }

    void LapisInstance::UpdateGlobalConstantBuffer()
    {
        gcb.elapsedTime = elapsedTime;
        gcb.deltaTime = deltaTime;

        float L = 0, T = 0, R = SCREEN_WIDTH, B = SCREEN_HEIGHT;
        DirectX::XMMATRIX m = {
             2.0f / (R - L),   0.0f,           0.0f,       0.0f ,
             0.0f,             2.0f / (T - B), 0.0f,       0.0f ,
             0.0f,             0.0f,           0.5f,       0.0f ,
             (R + L) / (L - R),(T + B) / (B - T),    0.5f,       1.0f 
        };
        auto screen = m;

        auto dxscreen = DirectX::XMMatrixOrthographicLH(SCREEN_WIDTH, SCREEN_HEIGHT, -10, 1000);

        

        auto world = DirectX::XMMatrixIdentity();

        DirectX::XMVECTOR Eye = Lapis::Helpers::XMVectorSet(Lapis::Vec4(0.0f, 0.0f, 0.0f, 0.0f));
        DirectX::XMVECTOR At = Lapis::Helpers::XMVectorSet(Lapis::Vec4(0.0f, 0.0f, 1.0f, 0.0f));
        DirectX::XMVECTOR Up = Lapis::Helpers::XMVectorSet(Lapis::Vec4(0.0f, 1.0f, 0.0f, 0.0f));
        auto view = DirectX::XMMatrixLookAtLH(Eye, At, Up);
        auto translateView = Lapis::Helpers::XMMatrixTranslation(this->cameraPosition);
        auto rotateView = DirectX::XMMatrixRotationY(this->CameraRotationY);
        rotateView = rotateView * DirectX::XMMatrixRotationX(-20 * DirectX::XM_PI / 180);
        view = view * translateView * rotateView;

        auto projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, 0.01f, 100.0f);

        gcb.Screen = screen;
        gcb.World = DirectX::XMMatrixTranspose(world);
        gcb.View = DirectX::XMMatrixTranspose(view);
        gcb.Projection = DirectX::XMMatrixTranspose(projection);

        RemapSubResource(constantBuffer, &gcb, sizeof(gcb));

    }
    void LapisInstance::RemapSubResource(ID3D11Resource* resource, void* data, size_t size)
    {
        D3D11_MAPPED_SUBRESOURCE ms;
        this->deviceContext->Map(resource, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
        memcpy(ms.pData, data, size);
        this->deviceContext->Unmap(resource, NULL);
    }
    void LapisInstance::PushVertex(Vertex vert) {
        if (this->VertexCount + 1 > this->VertexVectorCapacity) {
            this->VertexVectorCapacity += 1000;
            this->LapisVertexVector.reserve(this->VertexVectorCapacity);
        }
        this->LapisVertexVector.push_back(vert);
        this->VertexCount += 1;
    }
    void LapisInstance::PushCommand(LapisCommand lapisCommand) {
        this->LapisCommandVector.push_back(InternalLapisCommand(lapisCommand, this->VertexCount));
    }
    void LapisInstance::DrawCommand(InternalLapisCommand internalLapisCommand)
    {
        auto model = DirectX::XMMatrixIdentity();
        auto scaleModel = Lapis::Helpers::XMMatrixScaling(internalLapisCommand.transform.scale);
        auto rotateModel = Lapis::Helpers::XMMatrixRotationAxis(Lapis::Vec3(0, 1, 0), internalLapisCommand.transform.rot.y);
        auto translateModel = Lapis::Helpers::XMMatrixTranslation(internalLapisCommand.transform.pos);

        model = model * scaleModel * rotateModel * translateModel;
        gcb.Model = DirectX::XMMatrixTranspose(model);

        RemapSubResource(constantBuffer, &gcb, sizeof(gcb));

        auto& material = internalLapisCommand.material;
        static ID3D11VertexShader* prevVertexShader = nullptr;
        if (prevVertexShader != material->vertexShader) {
            deviceContext->VSSetShader(material->vertexShader, 0, 0);
            prevVertexShader = material->vertexShader;
        }
        static ID3D11PixelShader* prevPixelShader = nullptr;
        if (prevPixelShader != material->pixelShader) {
            deviceContext->PSSetShader(material->pixelShader, 0, 0);
            prevPixelShader = material->pixelShader;
        }

        this->deviceContext->IASetPrimitiveTopology(internalLapisCommand.topology);
        this->deviceContext->Draw(internalLapisCommand.vertexCount, internalLapisCommand.startVertexLocation);
    }
    void LapisInstance::InitDefaultShaders()
    {
#define CREATE_DEFAULT_SHADERS_SEPERATE(vsName, psName) \
        ID3D11VertexShader* vsName##_vertex; \
        this->device->CreateVertexShader(DEFAULTSHADER__##vsName##_vertex, sizeof(DEFAULTSHADER__##vsName##_vertex), NULL, &vsName##_vertex); \
        ID3D11PixelShader* psName##_pixel; \
        this->device->CreatePixelShader(DEFAULTSHADER__##psName##_pixel, sizeof(DEFAULTSHADER__##psName##_pixel), NULL, &psName##_pixel) \

#define CREATE_DEFAULT_SHADERS(name) CREATE_DEFAULT_SHADERS_SEPERATE(name, name);

#define CREATE_DEFAULT_MATERIAL_SEPERATE_SHADERS(name, vsName, psName) \
        CREATE_DEFAULT_SHADERS_SEPERATE(vsName, psName); \
        builtinMaterials.insert({ \
                #name,std::make_unique<Material>(vsName##_vertex, psName##_pixel, nullptr)\
            });

#define CREATE_DEFAULT_MATERIAL(name) CREATE_DEFAULT_MATERIAL_SEPERATE_SHADERS(name, name, name)
        

        CREATE_DEFAULT_MATERIAL(UI);
        CREATE_DEFAULT_MATERIAL_SEPERATE_SHADERS(UNLIT3D,UNLIT3D,UNLIT);

#undef CREATE_DEFAULT_SHADER
    }
    
}

