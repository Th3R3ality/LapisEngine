#include "D3.h"
#include "../Backend.h"

namespace Lapis
{
	namespace Draw
	{
		namespace D3
		{
			void Line(Vec3 from, Vec3 to, DXGI_RGBA rgba)
			{
				Backend::PushCommand(LapisCommand(2, D3D11_PRIMITIVE_TOPOLOGY_LINELIST, Transform(), "UNLIT3D"));

				Backend::PushVertex(Vertex(from, rgba, Vec2(0, 0), {}));
				Backend::PushVertex(Vertex(to, rgba, Vec2(1, 0), {}));
			}

			void Lines(std::vector<Vec3> points, DXGI_RGBA rgba)
			{
				Backend::PushCommand(LapisCommand((UINT)points.size(), D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP, Transform(), "UNLIT3D"));

				for (int idx = 0; idx < points.size(); idx++) {
					Backend::PushVertex(Vertex(points[idx], rgba, Vec2((float)idx / points.size(), 0), {}));
				}
			}

			void Triangle(Lapis::Transform transform, DXGI_RGBA rgba)
			{
				Backend::PushCommand(LapisCommand(3, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, transform, "UNLIT3D"));

				Backend::PushVertex(Vec3(0.0, 0.5, 0.0), rgba, Vec2(0.5, 1), -Vec3::forward);
				Backend::PushVertex(Vec3(0.5, -0.5, 0.0), rgba, Vec2(1, 0), -Vec3::forward);
				Backend::PushVertex(Vec3(-0.5, -0.5, 0.0), rgba, Vec2(0, 0), -Vec3::forward);
			}

			void Plane(Lapis::Transform transform, DXGI_RGBA rgba)
			{
				Backend::PushCommand(LapisCommand(4, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, transform, "UNLIT3D"));

				Backend::PushVertex(Vec3(-0.5, 0, 0.5), rgba, Vec2(-1, 0), Vec3::up);
				Backend::PushVertex(Vec3(0.5, 0, 0.5), rgba, Vec2(1, 0), Vec3::up);
				Backend::PushVertex(Vec3(-0.5, 0, -0.5), rgba, Vec2(-1, 1), Vec3::up);
				Backend::PushVertex(Vec3(0.5, 0, -0.5), rgba, Vec2(1, 1), Vec3::up);
			}

			void Cube(Lapis::Transform transform, DXGI_RGBA rgba)
			{
				Backend::PushCommand(LapisCommand(8, D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, transform, "UNLIT3D"));

				Backend::PushVertex(Vertex(Vec3(-.5f,  .5f, -.5f), rgba, Vec2(0,0), Vec3()));
				Backend::PushVertex(Vertex(Vec3(-.5f,  .5f,  .5f), rgba, Vec2(1,0), Vec3()));
				Backend::PushVertex(Vertex(Vec3( .5f,  .5f, -.5f), rgba, Vec2(0,0), Vec3()));
				Backend::PushVertex(Vertex(Vec3( .5f,  .5f,  .5f), rgba, Vec2(1,0), Vec3()));
				Backend::PushVertex(Vertex(Vec3( .5f, -.5f, -.5f), rgba, Vec2(0,0), Vec3()));
				Backend::PushVertex(Vertex(Vec3( .5f, -.5f,  .5f), rgba, Vec2(1,0), Vec3()));
				Backend::PushVertex(Vertex(Vec3(-.5f, -.5f, -.5f), rgba, Vec2(0,0), Vec3()));
				Backend::PushVertex(Vertex(Vec3(-.5f, -.5f,  .5f), rgba, Vec2(1,0), Vec3()));

				/*
				Backend::PushCommand(LapisCommand(8, D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, transform, "UNLIT3D"));

				Backend::PushVertex(0.5, 0.5, -0.5, rgba, { 0.0, 1.0, 0.0, 0.0 });
				Backend::PushVertex(0.5, -0.5, -0.5, rgba, { 0.0, 0.0, 0.0, 0.0 });
				Backend::PushVertex(-0.5, 0.5, -0.5, rgba, { 1.0, 1.0, 0.0, 0.0 });
				Backend::PushVertex(-0.5, -0.5, -0.5, rgba, { 1.0, 0.0, 0.0, 0.0 });
				Backend::PushVertex(-0.5, 0.5, 0.5, rgba, { 1.0, 0.0, 0.0, 0.0 });
				Backend::PushVertex(-0.5, -0.5, 0.5, rgba, { 1.0, 0.0, 0.0, 0.0 });
				Backend::PushVertex(0.5, 0.5, 0.5, rgba, { 1.0, 0.0, 0.0, 0.0 });
				Backend::PushVertex(0.5, -0.5, 0.5, rgba, { 1.0, 0.0, 0.0, 0.0 });
				*/

			}
		}
	}
}