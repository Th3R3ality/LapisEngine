#include "D2.h"
#define _USE_MATH_DEFINES
#include <math.h>

#include "../Backend.h"


namespace Lapis::Draw::D2
{
	void Point(Vec2 p, Color rgba)
	{
		Backend::PushCommand(LapisCommand(1, D3D_PRIMITIVE_TOPOLOGY_POINTLIST, Transform(), "UI"));

		Backend::PushVertex(Vertex(p, rgba, 0, -Vec3::forward));
	}

	void Line(Vec2 from, Vec2 to, Color rgba)
	{
		Backend::PushCommand(LapisCommand(2, D3D_PRIMITIVE_TOPOLOGY_LINELIST, Transform(), "UI"));

		Backend::PushVertex(Vertex({ from.x, from.y, 0 }, rgba, { 0,0 }, -Vec3::forward));
		Backend::PushVertex(Vertex({ to.x, to.y, 0 }, rgba, { 1,0 }, -Vec3::forward));
	}

	void Triangle(Vec2(p1), Vec2(p2), Vec2(p3), Color rgba)
	{
		Backend::PushCommand(LapisCommand(3, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, Transform(), "UI"));

		Backend::PushVertex(Vertex(p1, rgba, 0, -Vec3::forward));
		Backend::PushVertex(Vertex(p2, rgba, Vec2(0.5, 1), -Vec3::forward));
		Backend::PushVertex(Vertex(p3, rgba, Vec2(1, 0), -Vec3::forward));
	}

	void Rect(Vec2 tl, Vec2 wh, Color rgba)
	{
		Backend::PushCommand(LapisCommand(4, D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, Transform(), "UI"));

		Backend::PushVertex(Vertex({ tl.x, tl.y, 0 }, rgba, { 0,0 }, -Vec3::forward));
		Backend::PushVertex(Vertex({ tl.x + wh.x, tl.y, 0 }, rgba, { 1,0 }, -Vec3::forward));
		Backend::PushVertex(Vertex({ tl.x, tl.y + wh.y, 0 }, rgba, { 0,1 }, -Vec3::forward));
		Backend::PushVertex(Vertex({ tl.x + wh.x, tl.y + wh.y, 0 }, rgba, { 1,1 }, -Vec3::forward));
	}

	void Circle(Vec2 xy, float r, Color rgba)
	{
		
		Backend::PushCommand(LapisCommand(4, D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, Transform(), "CIRCLE"));

		Backend::PushVertex(Vertex(xy+Vec2(-1)*r, rgba, 0, -Vec3::forward));
		Backend::PushVertex(Vertex(xy+Vec2(1, -1)*r, rgba, Vec2(1, 0), -Vec3::forward));
		Backend::PushVertex(Vertex(xy+Vec2(-1, 1)*r, rgba, Vec2(0, 1), -Vec3::forward));
		Backend::PushVertex(Vertex(xy+Vec2(1)*r, rgba, 1, -Vec3::forward));
	}

	void Circle(Vec2 xy, float r, Color rgba, int vertexCount)
	{
		if (vertexCount < 6)
			return;
		r = r * 2;

		Backend::PushCommand(LapisCommand(vertexCount, D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, Transform(), "UI"));


		constexpr float pie = static_cast<float>((M_PI));

		for (int i = 0; i < vertexCount; i++) {
			float _x = cosf(i * pie / (vertexCount - 1)) * 0.5f + 0.5f;
			float _y = (i % 2 == 0 ? -1 : 1) * sinf(i * pie / (vertexCount - 1)) * 0.5f + 0.5f;
			Backend::PushVertex(Vertex(
				Vec3(xy.x + r * 2 * _x, xy.y + r * 2 * _y, 0),
				rgba,
				{ _x, _y }, -Vec3::forward)); //idk why this works but it makes the uv not inverted
		}
	}
}