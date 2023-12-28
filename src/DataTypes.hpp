#pragma once

#include <windows.h>
#include <windowsx.h>
#include <D3D11.h>
#include <DirectXMath.h>

#include "Material.h"

namespace Lapis
{
	struct Vec2
	{
		float x, y;

		Vec2 operator-() const;
		Vec2 operator+(const Vec2& other) const;
		Vec2 operator-(const Vec2& other) const;

		constexpr Vec2() :
			x(0), y(0)
		{}

		constexpr Vec2(float f) :
			x(f), y(f)
		{}

		constexpr Vec2(float x, float y) :
			x(x), y(y)
		{}

	};
	using Vector2 = Vec2;

	struct Vec3
	{
		float x, y, z;

		Vec3 operator-() const;

		constexpr Vec3() :
			x(0), y(0), z(0)
		{}

		constexpr Vec3(float f) :
			x(f), y(f), z(f)
		{}

		constexpr Vec3(float x, float y, float z) :
			x(x), y(y), z(z)
		{}
	};
	using Vector3 = Vec3;
	

	struct Vec4
	{
		float x, y, z, w;

		Vec4 operator+(const Vec4& other) const;
	};
	using Vector4 = Vec4;

	struct Vertex
	{
		union
		{
			Vec3 position;
			Vec3 pos;
		};
		union
		{
			DXGI_RGBA color;
			DXGI_RGBA col;
		};
		Vec2 uv;
		Vec3 normal;

		Vertex(Vec3 pos, DXGI_RGBA col, Vec2 uv, Vec3 normal) :
			pos(pos), col(col), uv(uv), normal(normal)
		{}
	};



	struct Transform {
		union
		{
			Vec3 position;
			Vec3 pos;
		};
		union
		{
			Vec3 rotation;
			Vec3 rot;
		};
		union
		{
			Vec3 scale;
		};
		Transform() :
			pos(0), rot(0), scale(0)
		{}
		Transform(Vec3 pos, Vec3 rot, Vec3 scale) :
			pos(pos), rot(rot), scale(scale)
		{}
	};

	struct LapisCommand
	{
		UINT vertexCount;
		D3D_PRIMITIVE_TOPOLOGY topology;
		Transform transform;
		Material material;

		LapisCommand(UINT vertexCount, D3D_PRIMITIVE_TOPOLOGY topology, Transform transform, Material material) :
			vertexCount(vertexCount), topology(topology), transform(transform), material(material)
		{}

		LapisCommand(UINT vertexCount, D3D_PRIMITIVE_TOPOLOGY topology, Material material) :
			vertexCount(vertexCount), topology(topology), material(material)
		{
			transform = Transform({}, {}, { 1,1,1 });
		}
	};

	struct InternalLapisCommand
	{
		UINT vertexCount;
		UINT startVertexLocation;
		D3D_PRIMITIVE_TOPOLOGY topology;
		Transform transform;
		Material material;

		InternalLapisCommand(LapisCommand drawCommand, UINT startVertexLocation)
			: vertexCount(drawCommand.vertexCount),
			startVertexLocation(startVertexLocation),
			topology(drawCommand.topology),
			transform(drawCommand.transform),
			material(drawCommand.material)
		{}
	};

	_declspec(align(16))
	struct GlobalConstantBuffer
	{
		float elapsedTime;
		float deltaTime;
		DirectX::XMMATRIX Screen;
		DirectX::XMMATRIX Model;
		DirectX::XMMATRIX World;
		DirectX::XMMATRIX View;
		DirectX::XMMATRIX Projection;
	};


}