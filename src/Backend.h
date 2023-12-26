#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <Windows.h>

#include <chrono>
#include <thread>
#include <vector>
#include <unordered_map>

#include "DataTypes.hpp"

#include "utils/hsl-to-rgb.hpp"

#define SCREEN_WIDTH (200*1)
#define SCREEN_HEIGHT (150*1)



namespace Lapis
{
	class LapisInstance
	{

	public:
		// global declarations
		

		Lapis::Vector3 cameraPosition = { 0, 0, 0 };
		float CameraRotationY = 0;

		float elapsedTime = 0;
		float deltaTime = 0;
		std::chrono::steady_clock::duration elapsedDuration;
		std::chrono::steady_clock::duration deltaDuration;

		LapisInstance()
		{
			swapchain = nullptr;
			device = nullptr;
			deviceContext = nullptr;
			backbuffer = nullptr;
			pLayout = nullptr;
			pVBuffer = nullptr;
			depthBufferView = nullptr;
			pConstantBuffer = nullptr;

			deltaDuration = {};
			elapsedDuration = deltaDuration;
			vertexBuffer.reserve(VBufferCapacity);
		}

		void Init();

		void InitD3D11(HWND hwnd);
		void CleanD3D11();

		void NewFrame();
		void RenderFrame();
		void FlushFrame();

		bool GetInput(int key);

		//void PushVertex(float x, float y, DXGI_RGBA col) { PushVertex(x, y, col, {0.5, 0.5, 0.5, 0.5}); };
		
		

		void DrawPoint(float x, float y, DXGI_RGBA rgba);
		void DrawPoint(float x, float y) { DrawPoint(x, y, { 1,0,1,1 }); };

		void DrawLine(Vector2 from, Vector2 to, DXGI_RGBA rgba);
		void DrawLine(float x1, float y1, float x2, float y2, DXGI_RGBA rgba) { DrawLine(Vector2(x1, y1), Vector2(x2, y2), rgba); };

		void DrawLine(float x1, float y1, float x2, float y2) { DrawLine(x1, y1, x2, y2, { 1,0,1,1 }); }

		void DrawRect(float x, float y, float w, float h, DXGI_RGBA rgba);
		void DrawRect(float x, float y, float w, float h, DXGI_RGB rgb = { 1, 0, 1 }, float alpha = 1.f) { DrawRect(x, y, w, h, { rgb.Red, rgb.Green, rgb.Blue, alpha }); }

		void DrawCircle(float x, float y, float w, float h, DXGI_RGBA rgba, int vertexCount = 6);


		void DrawTriangle3D(Lapis::Transform transform, DXGI_RGBA rgba);
		void DrawPlane(Lapis::Transform transform, DXGI_RGBA rgba);

		void DrawCube(Lapis::Transform transform, DXGI_RGBA rgba);

	private:

		IDXGISwapChain* swapchain; // the pointer to the swap chain interface
		ID3D11Device* device; // the pointer to our Direct3D device interface
		ID3D11DeviceContext* deviceContext; // the pointer to our Direct3D device context
		ID3D11RenderTargetView* backbuffer;    // global declaration
		ID3D11DepthStencilView* depthBufferView;
		ID3D11InputLayout* pLayout;

		ID3D11Buffer* pConstantBuffer;

		ID3D11Buffer* pVBuffer;

		std::chrono::steady_clock::duration initDuration = {};

		int VertexCount = 0;
		int VBufferCapacity = 1000;
		std::vector<VERTEX> vertexBuffer;
		std::vector<InternalLapisCommand> commandList;

		std::unordered_map<std::string, Material> builtinMaterials = {};

		void UpdateGlobalConstantBuffer();

		void RemapSubResource(ID3D11Resource* resource, void* data, size_t size);

		void PushVertex(VERTEX vert);
		void PushVertex(Vector3 pos, DXGI_RGBA col, Vector2 uv, Vector3 normal) { PushVertex(VERTEX(pos, col, uv, normal)); };
		void PushCommand(LapisCommand drawCommand);
		void DrawCommand(InternalLapisCommand internalDrawCommand);

		void InitDefaultShaders();
	};
}