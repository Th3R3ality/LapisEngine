#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include <vector>
#include <chrono>

#include "DataTypes.hpp"

#include "utils/hsl-to-rgb.hpp"

#define SCREEN_WIDTH 200
#define SCREEN_HEIGHT 150

namespace Lapis
{
	class LapisInstance
	{
	private:
		std::chrono::steady_clock::duration initDuration;


		int VerticeCount = 0;
		int VBufferCapacity = 1000;

	public:
		// global declarations
		IDXGISwapChain* swapchain; // the pointer to the swap chain interface
		ID3D11Device* device; // the pointer to our Direct3D device interface
		ID3D11DeviceContext* deviceContext; // the pointer to our Direct3D device context
		ID3D11RenderTargetView* backbuffer;    // global declaration
		ID3D11InputLayout* pLayout;
		
		ID3D11Buffer* pConstantBuffer;

		ID3D11VertexShader* pVS_unlit;    // the vertex shader
		ID3D11PixelShader* pPS_unlit;     // the pixel shader
		ID3D11Buffer* pVBuffer;

		std::vector<VERTEX> vertexBuffer;
		std::vector<LapisCommand> commandList;

		float elapsedTime = 0;
		float deltaTime = 0;
		std::chrono::steady_clock::duration elapsedDuration;
		std::chrono::steady_clock::duration deltaDuration;

		LapisInstance()
		{
			swapchain = 0;
			device = 0;
			deviceContext = 0;
			backbuffer = 0;
			pLayout = 0;
			pVS_unlit = 0;
			pPS_unlit = 0;
			pVBuffer = 0;

			deltaDuration = std::chrono::steady_clock::duration(0);
			elapsedDuration = deltaDuration;
			vertexBuffer.reserve(VBufferCapacity);
		}

		void Init();

		void InitD3D11(HWND hwnd);
		void CleanD3D11();

		void NewFrame();
		void RenderFrame();
		void CleanFrame();

		bool GetInput(int key);
		void InitPipeline();

		//void PushVertex(float x, float y, DXGI_RGBA col) { PushVertex(x, y, col, {0.5, 0.5, 0.5, 0.5}); };
		void PushVertex(float x, float y, DXGI_RGBA col, DirectX::XMFLOAT4 uv);
		void PushCommand(int VerticeCount, D3D_PRIMITIVE_TOPOLOGY Topology);

		void DrawPoint(float x, float y, DXGI_RGBA rgba);
		void DrawPoint(float x, float y) { DrawPoint(x, y, { 1,0,1,1 }); };

		void DrawLine(float x1, float y1, float x2, float y2, DXGI_RGBA rgba);
		void DrawLine(float x1, float y1, float x2, float y2) { DrawLine(x1, y1, x2, y2, { 1,0,1,1 }); }

		void DrawRect(float x, float y, float w, float h, DXGI_RGBA rgba);
		void DrawRect(float x, float y, float w, float h, DXGI_RGB rgb = { 1, 0, 1 }, float alpha = 1.f) { DrawRect(x, y, w, h, { rgb.Red, rgb.Green, rgb.Blue, alpha }); }

		void DrawCircle(float x, float y, float w, float h, DXGI_RGBA rgba, int vertexCount = 6);
	};

	
}