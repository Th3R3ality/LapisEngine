#include "Engine.h"

#include <iostream>
#include "Backend.h"

namespace Lapis
{

	float deltaTime;
	float elapsedTime;
	Transform mainCamera;
	Vec4 clientRect = { 0,0,100,100 };

	void InitLapis(IDXGISwapChain* swapchain)
	{
		std::cout << "Initting via SwapChain : " << swapchain << "\n";
		Backend::InitBackend(swapchain, nullptr);
	}
	void InitLapis(HWND hwnd)
	{
		std::cout << "initting via HWND : " << hwnd << "\n";
		Backend::InitBackend(nullptr, hwnd);
	}

	void WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		Backend::WndProcHandler(hwnd, msg, wParam, lParam);
	}

	void NewFrame()
	{
		Backend::NewFrame();
	}
	void RenderFrame()
	{
		Backend::RenderFrame();
	}
	void FlushFrame()
	{
		Backend::FlushFrame();
	}
	void PresentFrame()
	{
		Backend::PresentFrame();
	}
	void CleanLapis()
	{
		Backend::CleanD3D11();
	}

	void DestroyViews()
	{
		Backend::DestroyViews();
	}
	void CreateViews(IDXGISwapChain* swapchain)
	{
		Backend::CreateViews(swapchain);
	}

	void PushWorldMatrix(mat4x4 mat)
	{
		Backend::PushWorldMatrix(mat);
	}

	void PushViewMatrix(mat4x4 mat)
	{
		Backend::PushViewMatrix(mat);
	}

	void PushProjectionMatrix(mat4x4 mat)
	{
		Backend::PushProjectionMatrix(mat);
	}
	void GetDeviceAndCtx(ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext)
	{
		return Backend::GetDeviceAndCtx(ppDevice, ppContext);
	}

	HRESULT GetDeviceAndCtxFromSwapchain(IDXGISwapChain* pSwapChain, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext)
	{
		return Backend::GetDeviceAndCtxFromSwapchain(pSwapChain, ppDevice, ppContext);
	}
	ID3D11RenderTargetView* GetRenderTargetView()
	{
		return Backend::GetRenderTargetView();
	}

	void PushMaterial(Material mat)
	{
		Backend::PushMaterial(mat);
	}
	void PopMaterial()
	{
		Backend::PopMaterial();
	}
}