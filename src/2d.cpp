#include "2d.h"
#include "Backend.h"

namespace Lapis
{
	void LapisInstance::DrawPoint(float x, float y, DXGI_RGBA rgba) {
		this->PushCommand(1, D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

		this->PushVertex(x, y, rgba, { 0.5, 0.5, 0.5, 0.5 });
	}

	void LapisInstance::DrawLine(float x1, float y1, float x2, float y2, DXGI_RGBA rgba) {
		this->PushCommand(2, D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

		this->PushVertex(x1, y1, rgba, { 0.0, 0.0, 0.0, 0.0 });
		this->PushVertex(x2, y2, rgba, { 1.0, 1.0, 1.0, 1.0 });
	}

	void LapisInstance::DrawRect(float x, float y, float w, float h, DXGI_RGBA rgba)
	{
		this->PushCommand(4, D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		this->PushVertex(x, y + h, rgba, { 0.0, 1.0, 0.0, 0.0 });
		this->PushVertex(x, y, rgba, { 0.0, 0.0, 0.0, 0.0 });
		this->PushVertex(x + w, y + h, rgba, { 1.0, 1.0, 0.0, 0.0 });
		this->PushVertex(x + w, y, rgba, { 1.0, 0.0, 0.0, 0.0 });

	}

	void LapisInstance::DrawCircle(float x, float y, float w, float h, DXGI_RGBA rgba, int vertexCount){
		if (vertexCount < 6)
			return;

		this->PushCommand(vertexCount, D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		
#define _PI (float)3.14159265358979323846

		for (int i = 0; i < vertexCount; i++) {
			float _x = cosf(i * _PI / (vertexCount-1)) * 0.5 + 0.5;
			float _y = (i % 2 == 0 ? -1 : 1) * sinf(i * _PI / (vertexCount-1)) * 0.5 + 0.5;
			this->PushVertex(
				x + w * _x,
				y + h * _y,
				rgba, 
				{ _x, _y, 0.0, 0.0 }); //idk why this works but it makes the uv not inverted
		}
	}
}