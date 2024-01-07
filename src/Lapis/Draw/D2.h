#pragma once
#include "../LapisTypes.h"

namespace Lapis
{
	namespace Draw
	{
		namespace D2
		{
			void Point(Vec2 p, Color rgba);

			void Line(Vec2 from, Vec2 to, Color rgba);

			void Triangle(Vec2(p1), Vec2(p2), Vec2(p3), Color rgba);

			void Rect(Vec2 tl, Vec2 wh, Color rgba);
			inline void Rect(Vec4 ltrb, Color rgba) { Rect(Vec2(ltrb.x, ltrb.y), Vec2(ltrb.z - ltrb.x, ltrb.w - ltrb.y), rgba); };

			void Circle(Vec2 xy, float r, Color rgba);
			void Circle(Vec2 xy, float r, Color rgba, int vertexCount);
		}
	}
}