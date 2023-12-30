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


	void InitLapis(HWND hwnd);

	void NewFrame();
	void RenderFrame();
	void FlushFrame();

	void CleanLapis();
}