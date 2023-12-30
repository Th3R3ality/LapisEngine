#include "LapisEngine.h"
#include <iostream>
#include "Backend.h"

namespace Lapis
{

	float deltaTime;
	float elapsedTime;
	Transform mainCamera;


	void InitLapis(HWND hwnd)
	{
		std::cout << "hwnd: " << hwnd << "\n";
		Backend::InitBackend(hwnd);
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
	void CleanLapis()
	{
		Backend::CleanD3D11();
	}


}