#include "3d.h"
#include "Backend.h"

namespace Lapis
{
	void LapisInstance::DrawTriangle3D(Lapis::Vector3 pos, DXGI_RGBA rgba)
	{
		this->PushCommand(3, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, true);
		float elapsed = 0;

		if (LapisInstance::instance) {
			elapsed = LapisInstance::instance()->elapsedTime;
		}
		else
		{
			elapsed = 0;
		}

		this->PushVertex(pos.x, pos.y + 0.5, pos.z, rgba, {0.5, 1.0, 0.5, 0.0});
		this->PushVertex(pos.x + 0.5 * sinf(elapsed), pos.y - 0.5, pos.z + 0.5 * cosf(elapsed), rgba, {1, 0.0, 0.5, 0.0});
		this->PushVertex(pos.x - 0.5 * sinf(elapsed), pos.y - 0.5, pos.z - 0.5 * cosf(elapsed), rgba, {0, 0.0, 0.5, 0.0});

	}

	void LapisInstance::DrawPyramid(Lapis::Vector3 pos, DXGI_RGBA)
	{

	}

	void LapisInstance::DrawCube(float x, float y, float z, float scale, DXGI_RGBA rgba)
	{
		this->PushCommand(16, D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, true);

		this->PushVertex(x, y, z, rgba, { 0.0, 1.0, 0.0, 0.0 });
		this->PushVertex(x, y, z, rgba, { 0.0, 0.0, 0.0, 0.0 });
		this->PushVertex(x, y, z, rgba, { 1.0, 1.0, 0.0, 0.0 });
		this->PushVertex(x, y, z, rgba, { 1.0, 0.0, 0.0, 0.0 });

	}
}