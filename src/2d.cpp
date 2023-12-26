#include "2d.h"
#include "Backend.h"

namespace Lapis
{
	void LapisInstance::DrawPoint(float x, float y, DXGI_RGBA rgba)
	{

	}

	void LapisInstance::DrawLine(Vector2 from, Vector2 to, DXGI_RGBA rgba)
	{
		PushCommand(LapisCommand(2, D3D_PRIMITIVE_TOPOLOGY_LINELIST, builtinMaterials["UI"]));
		
		PushVertex(VERTEX({ from.x, from.y, 0 }, rgba, { 0,0 }, { 0,0,-1 }));
		PushVertex(VERTEX({ to.x, to.y, 0 }, rgba, { 1,0 }, { 0,0,-1 }));
	}

	void LapisInstance::DrawRect(Vector2 tl, Vector2 wh, DXGI_RGBA rgba)
	{
		PushCommand(LapisCommand(4, D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, builtinMaterials["UI"]));

		PushVertex(VERTEX({ tl.x, tl.y, 0 }, rgba, { 0,0 }, { 0,0,-1 }));
		PushVertex(VERTEX({ tl.x + wh.x, tl.y, 0 }, rgba, { 1,0 }, { 0,0,-1 }));
		PushVertex(VERTEX({ tl.x, tl.y + wh.y, 0 }, rgba, { 0,1 }, { 0,0,-1 }));
		PushVertex(VERTEX({ tl.x + wh.x, tl.y + wh.y, 0 }, rgba, { 1,1 }, { 0,0,-1 }));
	}

	void LapisInstance::DrawCircle(Vector2 xy, float r, DXGI_RGBA rgba, int vertexCount)
	{
		if (vertexCount < 6)
			return;

		PushCommand(LapisCommand(vertexCount, D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, builtinMaterials["UI"]));
		
#define _PI (float)3.14159265358979323846

		for (int i = 0; i < vertexCount; i++) {
			float _x = cosf(i * _PI / (vertexCount-1)) * 0.5 + 0.5;
			float _y = (i % 2 == 0 ? -1 : 1) * sinf(i * _PI / (vertexCount-1)) * 0.5 + 0.5;
			this->PushVertex(VERTEX(
				Vector3( xy.x + r*2 * _x, xy.y + r*2 * _y, 0),
				rgba,
				{ _x, _y }, { 0,0,-1 })); //idk why this works but it makes the uv not inverted
		}

#undef _PI
	}
}