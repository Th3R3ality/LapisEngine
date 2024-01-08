#pragma once
#include <vector>
#include "../LapisTypes.h"

namespace Lapis::Draw::D3
{
	void Line(Vec3 from, Vec3 to, Color rgba);
	void Lines(std::vector<Vec3> points, Color rgba);
	void Arrow(Vec3 point, Vec3 dir, float magnitude, Color rgba);
	inline void Arrow(Vec3 point, Vec3 dir, Color rgba) { Arrow(point, dir, 1, rgba); }
	void Triangle(Lapis::Transform transform, Color rgba);
	void Plane(Lapis::Transform transform, Color rgba);
	void Cube(Lapis::Transform transform, Color rgba);
	void Icosahedron(Transform transform, Color rgba);
}