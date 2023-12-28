#pragma once
#include "LapisTypes.h"

namespace Lapis
{
	namespace D2
	{
		void DrawPoint(float x, float y, DXGI_RGBA rgba);
		void DrawLine(Vec2 from, Vec2 to, DXGI_RGBA rgba);
		void DrawRect(Vec2 tl, Vec2 wh, DXGI_RGBA rgba);
		void DrawCircle(Vec2 xy, float r, DXGI_RGBA rgba, int vertexCount);
	}
}