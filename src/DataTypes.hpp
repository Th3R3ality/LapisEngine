#pragma once

#include <windows.h>
#include <windowsx.h>
#include <D3D11.h>
#include <DirectXMath.h>

#include "Material.h"

namespace Lapis
{
	struct Vector2
	{
		float x, y;

		Vector2 operator-() const;
		Vector2 operator+(const Vector2& other) const;
		Vector2 operator-(const Vector2& other) const;
	};

	struct Vector3
	{
		float x, y, z;

		Vector3 operator-() const;

		constexpr Vector3() :
			x(0), y(0), z(0)
		{}

		constexpr Vector3(float f) :
			x(f), y(f), z(f)
		{}

		constexpr Vector3(float x, float y, float z) :
			x(x), y(y), z(z)
		{}
	};

	struct Vector4
	{
		float x, y, z, w;

		Vector4 operator+(const Vector4& other) const;
	};

	struct VERTEX
	{
		union
		{
			Vector3 position;
			Vector3 pos;
		};
		union
		{
			DXGI_RGBA color;
			DXGI_RGBA col;
		};
		Vector2 uv;
		Vector3 normal;

		VERTEX(Vector3 pos, DXGI_RGBA col, Vector2 uv, Vector3 normal) :
			pos(pos), col(col), uv(uv), normal(normal)
		{}
	};



	struct Transform {
		union
		{
			Vector3 position;
			Vector3 pos;
		};
		union
		{
			Vector3 rotation;
			Vector3 rot;
		};
		union
		{
			Vector3 scale;
		};
		Transform() :
			pos(0), rot(0), scale(0)
		{}
		Transform(Vector3 pos, Vector3 rot, Vector3 scale) :
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
		float fTime;
		DirectX::XMMATRIX Screen;
		DirectX::XMMATRIX Model;
		DirectX::XMMATRIX World;
		DirectX::XMMATRIX View;
		DirectX::XMMATRIX Projection;
	};


}