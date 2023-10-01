#include "2d.h"
#include "Backend.h"

namespace Lapis
{
	void LapisInstance::DrawPoint(float x, float y, DXGI_RGBA rgba) {
		this->PushCommand(1, D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);

		this->PushVertex(x, y, rgba, { 0.5, 0.5, 0.5, 0.5 });
	}

	void LapisInstance::DrawLine(float x1, float y1, float x2, float y2, DXGI_RGBA rgba) {
		this->PushCommand(2, D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST);

		this->PushVertex(x1, y1, rgba, { 0.0, 0.0, 0.0, 0.0 });
		this->PushVertex(x2, y2, rgba, { 1.0, 1.0, 1.0, 1.0 });
	}

	void LapisInstance::DrawRect(float x, float y, float w, float h, DXGI_RGBA rgba)
	{
		this->PushCommand(4, D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		this->PushVertex(x, y, rgba, { 0.0, 0.0, 0.0, 0.0 });
		this->PushVertex(x, y + h, rgba, { 1.0, 0.0, 0.0, 0.0 });
		this->PushVertex(x + w, y, rgba, { 0.0, 1.0, 0.0, 0.0 });
		this->PushVertex(x + w, y + h, rgba, { 1.0, 1.0, 0.0, 0.0 });

	}
}