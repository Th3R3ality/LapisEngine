#pragma once
#include <vector>
#include "LapisTypes.h"

namespace Lapis 
{
	namespace D3
	{
		void DrawLine(Vec3 from, Vec3 to, DXGI_RGBA rgba);
		void DrawLines(std::vector<Vec3> points, DXGI_RGBA rgba);
		void DrawTriangle(Lapis::Transform transform, DXGI_RGBA rgba);
		void DrawPlane(Lapis::Transform transform, DXGI_RGBA rgba);
		void DrawCube(Lapis::Transform transform, DXGI_RGBA rgba);
	}
}