#pragma once
#include <Windows.h>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include <chrono>
#include <thread>
#include <vector>
#include <unordered_map>

#include "LapisTypes.h"
#include "LapisInternalTypes.h"

#include "../utils/hsl-to-rgb.hpp"

#define SCREEN_WIDTH (200*1)
#define SCREEN_HEIGHT (150*1)



namespace Lapis
{
	namespace Backend
	{
		void InitD3D11(HWND hwnd);

		//void UpdateGlobalConstantBuffer();
		//void RemapSubResource(ID3D11Resource* resource, void* data, size_t size);
		//void PushVertex(Vertex vert);
		//void PushVertex(Vec3 pos, Color col, Vec2 uv, Vec3 normal) { PushVertex(Vertex(pos, col, uv, normal)); };
		//void PushCommand(LapisCommand lapisCommand);
		//void DrawCommand(InternalLapisCommand internalLapisCommand);
		//void InitDefaultShaders();
	}

	class LapisInstance
	{

	public:
		// global declarations

		Lapis::Vec3 cameraPosition = { 0, 0, 0 };
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
			frameBuffer = nullptr;
			inputLayout = nullptr;
			vertexBuffer = nullptr;
			depthBufferView = nullptr;
			constantBuffer = nullptr;

			deltaDuration = {};
			elapsedDuration = deltaDuration;
			LapisVertexVector.reserve(VertexVectorCapacity);
		}

		void Init();

		void InitD3D11(HWND hwnd);
		void CleanD3D11();

		void NewFrame();
		void RenderFrame();
		void FlushFrame();

		void DrawPoint(float x, float y, DXGI_RGBA rgba);
		void DrawPoint(float x, float y) { DrawPoint(x, y, { 1,0,1,1 }); };
		void DrawLine(Vec2 from, Vec2 to, DXGI_RGBA rgba);
		void DrawRect(Vec2 xy, Vec2 wh, DXGI_RGBA rgba);
		void DrawRect(Vec4 ltrb, DXGI_RGBA rgba) { DrawRect(Vec2(ltrb.x, ltrb.y), Vec2(ltrb.z - ltrb.x, ltrb.w - ltrb.y), rgba); };
		void DrawCircle(Vec2 xy, float r, Color rgba);
		void DrawCircle(Vec2 xy, float r, DXGI_RGBA rgba, int vertexCount);

		void DrawLine3D(Vec3 from, Vec3 to, DXGI_RGBA rgba);
		void DrawLines3D(std::vector<Vec3> points, DXGI_RGBA rgba);
		void DrawTriangle3D(Lapis::Transform transform, DXGI_RGBA rgba);
		void DrawPlane(Lapis::Transform transform, DXGI_RGBA rgba);
		void DrawCube(Lapis::Transform transform, DXGI_RGBA rgba);


	private:

		IDXGISwapChain* swapchain; // the pointer to the swap chain interface
		ID3D11Device* device; // the pointer to our Direct3D device interface
		ID3D11DeviceContext* deviceContext; // the pointer to our Direct3D device context
		ID3D11RenderTargetView* frameBuffer;    // global declaration
		ID3D11DepthStencilView* depthBufferView;
		ID3D11InputLayout* inputLayout;
		ID3D11Buffer* constantBuffer;
		ID3D11Buffer* vertexBuffer;
		ID3D11BlendState* blendState;

		GlobalConstantBuffer gcb{};


		std::chrono::steady_clock::duration initDuration = {};

		int VertexCount = 0;
		int VertexVectorCapacity = 1000;
		std::vector<Vertex> LapisVertexVector;
		std::vector<InternalLapisCommand> LapisCommandVector;

		std::unordered_map<std::string, std::shared_ptr<Material>> builtinMaterials = {};

		

		void UpdateGlobalConstantBuffer();
		void RemapSubResource(ID3D11Resource* resource, void* data, size_t size);
		void PushVertex(Vertex vert);
		void PushVertex(Vec3 pos, DXGI_RGBA col, Vec2 uv, Vec3 normal) { PushVertex(Vertex(pos, col, uv, normal)); };
		void PushCommand(LapisCommand lapisCommand);
		void DrawCommand(InternalLapisCommand internalLapisCommand);
		void InitDefaultShaders();
	};
}