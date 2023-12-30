#include "Backend.h"

#include <iostream>

#include "GlobalDefines.h"

#include "shaders/DefaultShaders.h"
#include "LapisEngine.h"
#include "../utils/hsl-to-rgb.hpp"

namespace Lapis
{
	namespace Backend
	{
		IDXGISwapChain* swapchain; // the pointer to the swap chain interface
		ID3D11Device* device; // the pointer to our Direct3D device interface
		ID3D11DeviceContext* deviceContext; // the pointer to our Direct3D device context
		ID3D11RenderTargetView* frameBuffer;    // global declaration
		ID3D11DepthStencilView* depthBufferView;
		ID3D11InputLayout* inputLayout;
		ID3D11Buffer* constantBuffer;
		ID3D11Buffer* vertexBuffer;
		ID3D11BlendState* blendState;

		GlobalConstantBuffer gcb;

		int VertexCount = 0;
		int VertexVectorCapacity = 1000;
        std::vector<Vertex> LapisVertexVector{};
        std::vector<InternalLapisCommand> LapisCommandVector{};

        std::chrono::steady_clock::duration elapsedDuration{};
        std::chrono::steady_clock::duration deltaDuration{};
        std::chrono::steady_clock::duration initDuration{};
		std::unordered_map<std::string, std::shared_ptr<InternalMaterial>> builtinMaterials{};


        void InitBackend(HWND hwnd)
        {
            initDuration = std::chrono::high_resolution_clock::now().time_since_epoch();
            InitD3D11(hwnd);
        }
        void InitD3D11(HWND hwnd)
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
                &swapchain,
                &device,
                NULL,
                &deviceContext);

            std::cout << "created device and swapchain\n";

            // get the address of the back buffer
            ID3D11Texture2D* pBackBuffer = nullptr;
            swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

            // use the back buffer address to create the render target
            device->CreateRenderTargetView(pBackBuffer, NULL, &frameBuffer);
            pBackBuffer->Release();

            // set the render target as the back buffer
            deviceContext->OMSetRenderTargets(1, &frameBuffer, NULL);

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

            device->CreateInputLayout(ied, sizeof(ied) / sizeof(ied[0]), DEFAULTSHADER__UI_vertex, sizeof(DEFAULTSHADER__UI_vertex), &inputLayout);
            deviceContext->IASetInputLayout(inputLayout);


            D3D11_BUFFER_DESC cbDesc{};
            cbDesc.ByteWidth = sizeof(GlobalConstantBuffer);
            cbDesc.Usage = D3D11_USAGE_DYNAMIC;
            cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            cbDesc.MiscFlags = 0;
            cbDesc.StructureByteStride = 0;

            device->CreateBuffer(&cbDesc, NULL, &constantBuffer);
            deviceContext->VSSetConstantBuffers(0, 1, &constantBuffer);
            deviceContext->PSSetConstantBuffers(0, 1, &constantBuffer);

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
            device->CreateRasterizerState(&rasterizerDesc, &pRasterizerState);
            deviceContext->RSSetState(pRasterizerState);

            D3D11_TEXTURE2D_DESC depthBufferDesc;
            pBackBuffer->GetDesc(&depthBufferDesc); // copy from framebuffer properties
            depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
            depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
            ID3D11Texture2D* depthBuffer;
            device->CreateTexture2D(&depthBufferDesc, nullptr, &depthBuffer);
            device->CreateDepthStencilView(depthBuffer, nullptr, &depthBufferView);

            deviceContext->OMSetRenderTargets(1, &frameBuffer, depthBufferView);

            D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
            depthStencilDesc.DepthEnable = TRUE;
            depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
            depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
            ID3D11DepthStencilState* depthStencilState;
            device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);

            deviceContext->OMSetDepthStencilState(depthStencilState, 0);

            D3D11_BLEND_DESC desc;
            ZeroMemory(&desc, sizeof(desc));
            desc.AlphaToCoverageEnable = false;
            desc.RenderTarget[0].BlendEnable = true;
            desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
            desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
            desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
            desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_DEST_ALPHA;
            desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
            desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
            desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
            device->CreateBlendState(&desc, &blendState);

            deviceContext->OMSetBlendState(blendState, nullptr, 0xffffffff); // use default blend mode (i.e. disable)
        }
        void CleanD3D11()
        {
            swapchain->SetFullscreenState(0, NULL);

            safe_release(inputLayout);

            builtinMaterials.clear();

            safe_release(vertexBuffer);
            safe_release(frameBuffer);

            safe_release(swapchain);
            safe_release(device);
            safe_release(deviceContext);
        }

        void NewFrame()
        {
            static auto old = initDuration;

            auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
            deltaDuration = (now - old);
            elapsedDuration = (now - initDuration);
            old = now;

            using s = std::chrono::duration<float>;
            Lapis::deltaTime = std::chrono::duration_cast<s>(deltaDuration).count();// / 1000 / 1000;
            Lapis::elapsedTime += Lapis::deltaTime;
        }
        void RenderFrame()
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


                device->CreateBuffer(&bd, NULL, &vertexBuffer);       // create the buffer
                if (!vertexBuffer)
                    return;

                VBufferSize = VertexVectorCapacity;
            }

            RemapSubResource(vertexBuffer, LapisVertexVector.data(), sizeof(Vertex) * LapisVertexVector.size());

#if CLEAR_RENDER_TARGET_VIEW == 1
            static float h = 0;
            h += Lapis::deltaTime;
            if (h > 360) h -= 360;
            auto color = hsl2rgb((int)h, 1.0f, 0.7f, 1.0f);
            deviceContext->ClearRenderTargetView(frameBuffer, (FLOAT*)&color);
#endif
#if CLEAR_DEPTH_STENCIL_VIEW == 1
            deviceContext->ClearDepthStencilView(depthBufferView, D3D11_CLEAR_DEPTH, 1.0f, 0);
#endif

            UINT stride = sizeof(Vertex);
            UINT offset = 0;
            deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

            UpdateGlobalConstantBuffer();

            for (auto& internalCommand : LapisCommandVector) {
                {
                    DrawCommand(internalCommand);
                }
            }
            swapchain->Present(0, 0);
        }
        void FlushFrame()
        {
            LapisVertexVector.clear();
            VertexCount = 0;

            LapisCommandVector.clear();
        }

        void BeginCommand(Topology topology, std::string materialName)
        {
            UNIMPLEMENTED(BeginCommand);
        }
        void EndCommand()
        {
            UNIMPLEMENTED(EndCommand);
        }

        void PushVertex(Vertex vert)
        {
            if (VertexCount + 1 > VertexVectorCapacity) {
                VertexVectorCapacity += 1000;
                LapisVertexVector.reserve(VertexVectorCapacity);
            }

            LapisVertexVector.push_back(vert);
            VertexCount += 1;
        }
        void PushCommand(LapisCommand lapisCommand)
        {
            LapisCommandVector.push_back(InternalLapisCommand(lapisCommand, VertexCount));
        }

        void UpdateGlobalConstantBuffer()
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
#if USE_Z_UP
            world = world * Lapis::Helpers::XMMatrixRotationAxis(Vec3::right, 90 * DEG2RAD);
            world = world * DirectX::XMMatrixScaling(1, 1, -1);
#endif

            DirectX::XMVECTOR Eye = Helpers::XMVectorSet(0);
            DirectX::XMVECTOR At = Helpers::XMVectorSet(Vec3::forward);
            DirectX::XMVECTOR Up = Helpers::XMVectorSet(Vec3::up);
            auto view = DirectX::XMMatrixLookAtLH(Eye, At, Up);
            auto translateView = Helpers::XMMatrixTranslation(mainCamera.pos);
            auto rotateView = Helpers::XMMatrixRotationRollPitchYaw(-mainCamera.rotation);
            auto scaleView = Helpers::XMMatrixScaling(mainCamera.scale);
            view = view * translateView * rotateView;

            auto projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, 0.01f, 100.0f);

            gcb.Screen = screen;
            gcb.World = DirectX::XMMatrixTranspose(world);
            gcb.View = DirectX::XMMatrixTranspose(view);
            gcb.Projection = DirectX::XMMatrixTranspose(projection);

            RemapSubResource(constantBuffer, &gcb, sizeof(gcb));

        }
        void DrawCommand(InternalLapisCommand internalLapisCommand)
        {
            auto model = DirectX::XMMatrixIdentity();
            auto scaleModel = Helpers::XMMatrixScaling(internalLapisCommand.transform.scale);
            auto rotateModel = Helpers::XMMatrixRotationRollPitchYaw(internalLapisCommand.transform.rot);
            auto translateModel = Helpers::XMMatrixTranslation(internalLapisCommand.transform.pos);

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

            deviceContext->IASetPrimitiveTopology(internalLapisCommand.topology);
            deviceContext->Draw(internalLapisCommand.vertexCount, internalLapisCommand.startVertexLocation);
        }
        void InitDefaultShaders()
        {
#define CREATE_DEFAULT_SHADERS_SEPERATE(vsName, psName) \
        ID3D11VertexShader* vsName##_vertex; \
        device->CreateVertexShader(DEFAULTSHADER__##vsName##_vertex, sizeof(DEFAULTSHADER__##vsName##_vertex), NULL, &vsName##_vertex); \
        ID3D11PixelShader* psName##_pixel; \
        device->CreatePixelShader(DEFAULTSHADER__##psName##_pixel, sizeof(DEFAULTSHADER__##psName##_pixel), NULL, &psName##_pixel) \

#define CREATE_DEFAULT_SHADERS(name) CREATE_DEFAULT_SHADERS_SEPERATE(name, name);

#define CREATE_DEFAULT_MATERIAL_SEPERATE_SHADERS(name, vsName, psName) \
    { \
        CREATE_DEFAULT_SHADERS_SEPERATE(vsName, psName); \
            builtinMaterials.insert({ \
                    #name,std::make_unique<InternalMaterial>(#name, vsName##_vertex, psName##_pixel, nullptr)\
                }); \
    }

#define CREATE_DEFAULT_MATERIAL(name) CREATE_DEFAULT_MATERIAL_SEPERATE_SHADERS(name, name, name)


            CREATE_DEFAULT_MATERIAL(UI);
            CREATE_DEFAULT_MATERIAL_SEPERATE_SHADERS(UNLIT3D, UNLIT3D, UNLIT);
            CREATE_DEFAULT_MATERIAL_SEPERATE_SHADERS(CIRCLE, UI, CIRCLE);

#undef CREATE_DEFAULT_SHADER
        }
        void RemapSubResource(ID3D11Resource* resource, void* data, size_t size)
        {
            D3D11_MAPPED_SUBRESOURCE ms;
            deviceContext->Map(resource, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
            memcpy(ms.pData, data, size);
            deviceContext->Unmap(resource, NULL);
        }
	}
}