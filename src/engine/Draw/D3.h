#pragma once
#include <vector>
#include "../LapisTypes.h"

namespace Lapis
{
	namespace Draw
	{
		namespace D3
		{
			void Line(Vec3 from, Vec3 to, Color rgba);
			void Lines(std::vector<Vec3> points, Color rgba);
			void Triangle(Lapis::Transform transform, Color rgba);
			void Plane(Lapis::Transform transform, Color rgba);
			void DrawCube(Lapis::Transform transform, Color rgba);
		}
	}
}
