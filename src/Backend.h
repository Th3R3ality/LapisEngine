#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include "DataTypes.hpp"

#include "utils/hsl-to-rgb.hpp"

#define SCREEN_WIDTH 200
#define SCREEN_HEIGHT 150

namespace Lapis
{
	class LapisInstance
	{
	public:
		// global declarations
		IDXGISwapChain* swapchain; // the pointer to the swap chain interface
		ID3D11Device* device; // the pointer to our Direct3D device interface
		ID3D11DeviceContext* deviceContext; // the pointer to our Direct3D device context
		ID3D11RenderTargetView* backbuffer;    // global declaration
		ID3D11InputLayout* pLayout;
		ID3D11VertexShader* pVS_unlit;    // the vertex shader
		ID3D11PixelShader* pPS_unlit;     // the pixel shader
		ID3D11Buffer* pVBuffer;

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
		}

		void Init();

		void InitD3D11(HWND hwnd);
		void CleanD3D11();

		void RenderFrame();
		void InitPipeline();
		void InitGraphics();
	};

	
}