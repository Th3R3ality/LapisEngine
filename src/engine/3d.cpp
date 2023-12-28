#include "3d.h"
#include "Backend.h"

namespace Lapis
{
	void LapisInstance::DrawLine3D(Vec3 from, Vec3 to, DXGI_RGBA rgba)
	{
		PushCommand(LapisCommand(2, D3D11_PRIMITIVE_TOPOLOGY_LINELIST, builtinMaterials["UNLIT3D"]));

		PushVertex(Vertex(from, rgba, Vec2(0, 0), {}));
		PushVertex(Vertex(to, rgba, Vec2(1, 0), {}));
	}

	void LapisInstance::DrawLines3D(std::vector<Vec3> points, DXGI_RGBA rgba)
	{
		PushCommand(LapisCommand(points.size(), D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP, builtinMaterials["UNLIT3D"]));

		for (int idx = 0; idx < points.size(); idx++) {
			PushVertex(Vertex(points[idx], rgba, Vec2((float)idx/points.size(), 0), {}));
		}
	}

	void LapisInstance::DrawTriangle3D(Lapis::Transform transform, DXGI_RGBA rgba)
	{
		PushCommand(LapisCommand(3, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, transform, builtinMaterials["UNLIT3D"]));

		PushVertex(Vec3(0.0, 0.5, 0.0), rgba, Vec2(0.5, 1), Vec3(0, 0, -1));
		PushVertex(Vec3(0.5, -0.5, 0.0), rgba, Vec2(1, 0), Vec3(0, 0, -1));
		PushVertex(Vec3(-0.5, -0.5, 0.0), rgba, Vec2(0, 0), Vec3(0, 0, -1));
	}

	void LapisInstance::DrawPlane(Lapis::Transform transform, DXGI_RGBA rgba)
	{
		PushCommand(LapisCommand(4, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, transform, builtinMaterials["UNLIT3D"]));

		PushVertex(Vec3(-0.5, 0.0, 0.5), rgba, Vec2(-1, 0), Vec3(0, 1, 0));
		PushVertex(Vec3(0.5, 0.0, 0.5), rgba, Vec2(1, 0), Vec3(0, 1, 0));
		PushVertex(Vec3(-0.5, 0.0, -0.5), rgba, Vec2(-1, 1), Vec3(0, 1, 0));
		PushVertex(Vec3(0.5, 0.0, -0.5), rgba, Vec2(1, 1), Vec3(0, 1, 0));
	}

	/*void LapisInstance::DrawCube(Lapis::Transform transform, DXGI_RGBA rgba)
	{
		this->PushCommand(8, D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, true);
		this->PushTransform(transform);

		this->PushVertex(-0.5, 0.5, -0.5, rgba, { 0.0, 1.0, 0.0, 0.0 });
		this->PushVertex(-0.5, 0.5, 0.5, rgba, { 0.0, 0.0, 0.0, 0.0 });
		this->PushVertex(0.5, 0.5, -0.5, rgba, { 1.0, 1.0, 0.0, 0.0 });
		this->PushVertex(0.5, 0.5, 0.5, rgba, { 1.0, 0.0, 0.0, 0.0 });
		this->PushVertex(0.5, -0.5, -0.5, rgba, { 1.0, 0.0, 0.0, 0.0 });
		this->PushVertex(0.5, -0.5, 0.5, rgba, { 1.0, 0.0, 0.0, 0.0 });
		this->PushVertex(-0.5, -0.5, -0.5, rgba, { 1.0, 0.0, 0.0, 0.0 });
		this->PushVertex(-0.5, -0.5, 0.5, rgba, { 1.0, 0.0, 0.0, 0.0 });

		this->PushCommand(8, D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, true);
		this->PushTransform(transform);

		this->PushVertex(0.5, 0.5, -0.5, rgba, { 0.0, 1.0, 0.0, 0.0 });
		this->PushVertex(0.5, -0.5, -0.5, rgba, { 0.0, 0.0, 0.0, 0.0 });
		this->PushVertex(-0.5, 0.5, -0.5, rgba, { 1.0, 1.0, 0.0, 0.0 });
		this->PushVertex(-0.5, -0.5, -0.5, rgba, { 1.0, 0.0, 0.0, 0.0 });
		this->PushVertex(-0.5, 0.5, 0.5, rgba, { 1.0, 0.0, 0.0, 0.0 });
		this->PushVertex(-0.5, -0.5, 0.5, rgba, { 1.0, 0.0, 0.0, 0.0 });
		this->PushVertex(0.5, 0.5, 0.5, rgba, { 1.0, 0.0, 0.0, 0.0 });
		this->PushVertex(0.5, -0.5, 0.5, rgba, { 1.0, 0.0, 0.0, 0.0 });

	}*/
}