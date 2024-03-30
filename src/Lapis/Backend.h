#pragma once

#include <d3d11.h>
#include <vector>
#include <unordered_map>
#include <memory>
#include <string>
#include <chrono>


#include "LapisTypes.h"
#include "InternalTypes.h"

namespace Lapis
{
	namespace Backend
	{
		//extern IDXGISwapChain* swapchain; // the pointer to the swap chain interface
		//extern ID3D11Device* device; // the pointer to our Direct3D device interface
		//extern ID3D11DeviceContext* deviceContext; // the pointer to our Direct3D device context
		//extern ID3D11RenderTargetView* frameBuffer;    // global declaration
		//extern ID3D11DepthStencilView* depthBufferView;
		//extern ID3D11InputLayout* inputLayout;
		//extern ID3D11Buffer* constantBuffer;
		//extern ID3D11Buffer* vertexBuffer;
		//extern ID3D11BlendState* blendState;

		//extern GlobalConstantBuffer gcb;

		//extern int VertexCount;
		//extern int VertexVectorCapacity;
		//extern std::vector<Vertex> LapisVertexVector;
		//extern std::vector<InternalLapisCommand> LapisCommandVector;

		extern std::chrono::steady_clock::duration elapsedDuration;
		extern std::chrono::steady_clock::duration deltaDuration;
		extern std::chrono::steady_clock::duration initDuration;

		extern std::unordered_map<std::string, std::shared_ptr<InternalMaterial>> builtinMaterials;

		void InitBackend(IDXGISwapChain* _swapchain, HWND _hwnd);
		void InitD3D11(IDXGISwapChain* _swapchain, HWND _hwnd);
		void SetupD3D11State();
		void CleanD3D11();

		void WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		void NewFrame();
		void RenderFrame();
		void FlushFrame();
		void PresentFrame();

		void DestroyViews();
		bool CreateViews(IDXGISwapChain* _swapchain);

		void PushWorldMatrix(mat4x4 mat);
		void PushViewMatrix(mat4x4 mat);
		void PushProjectionMatrix(mat4x4 mat);

		void PushVertex(Vertex vert);
		inline void PushVertex(Vec3 pos, Color col, Vec2 uv, Vec3 normal) { PushVertex(Vertex(pos, col, uv, normal)); };
		void PushCommand(LapisCommand lapisCommand);

		void UpdateGlobalConstantBuffer();
		void DrawCommand(InternalLapisCommand internalLapisCommand);
		void InitDefaultShaders();
		void MapResource(ID3D11Resource* resource, void* data, size_t size);

		void GetDeviceAndCtx(ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext);
		HRESULT GetDeviceAndCtxFromSwapchain(IDXGISwapChain* pSwapChain, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext);
		ID3D11RenderTargetView* GetRenderTargetView();

		void PushMaterial(Material mat);
		void PopMaterial();
	}
}