#pragma once
#include <Windows.h>
#include "LapisTypes.h"
#include "Draw/D2.h"
#include "Draw/D3.h"

namespace Lapis
{

	extern float deltaTime;
	extern float elapsedTime;
	extern Transform mainCamera;
	extern Vec4 clientRect;
	

	void InitLapis(IDXGISwapChain* swapchain);
	void InitLapis(HWND hwnd);

	void WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	void NewFrame();
	void RenderFrame();
	void FlushFrame();

	void CleanLapis();

	void DestroyViews();
	void CreateViews(IDXGISwapChain* swapchain);

	void PushWorldMatrix(mat4x4 mat);
	void PushViewMatrix(mat4x4 mat);
	void PushProjectionMatrix(mat4x4 mat);
}