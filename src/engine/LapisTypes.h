#pragma once

#include <d3d11.h>
#include <memory>
#include <string>

namespace Lapis
{
	using Color = DXGI_RGBA;

	struct Vec2
	{
		float x, y;

		Vec2 operator-() const;
		Vec2 operator+(const Vec2& other) const;
		Vec2 operator-(const Vec2& other) const;
		Vec2 operator*(const float& mul) const;

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
		Vec3 operator+(const Vec3& other) const;
		Vec3 operator*(const float& scalar) const;

		Vec3& operator+=(const Vec3& other);
		Vec3& operator-=(const Vec3& other);


		constexpr Vec3() :
			x(0), y(0), z(0)
		{}

		constexpr Vec3(float f) :
			x(f), y(f), z(f)
		{}

		constexpr Vec3(float x, float y, float z) :
			x(x), y(y), z(z)
		{}

		constexpr Vec3(Vec2 v2) :
			x(v2.x), y(v2.y), z(0)
		{}

		static const Vec3 up;
		static const Vec3 right;
		static const Vec3 forward;
	};
	using Vector3 = Vec3;


	struct Vec4
	{
		float x, y, z, w;

		Vec4 operator+(const Vec4& other) const;

		constexpr Vec4() :
			x(0), y(0), z(0), w(0)
		{}
		constexpr Vec4(float f) :
			x(f), y(f), z(f), w(f)
		{}
		constexpr Vec4(float x, float y, float z, float w) :
			x(x), y(y), z(z), w(w)
		{}

		constexpr Vec4(Vec3 v3, float w) :
			x(v3.x), y(v3.y), z(v3.z), w(w)
		{}
		constexpr Vec4(Vec3 v3) :
			x(v3.x), y(v3.y), z(v3.z), w(0)
		{}
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

	struct Transform
	{
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
			pos(0), rot(0), scale(1)
		{}
		Transform(Vec3 pos, Vec3 rot, Vec3 scale) :
			pos(pos), rot(rot), scale(scale)
		{}
	};


	/*struct LapisCommand
	{
		UINT vertexCount;
		D3D_PRIMITIVE_TOPOLOGY topology;
		Transform transform;
		std::shared_ptr<Material> material;

		LapisCommand(UINT vertexCount, D3D_PRIMITIVE_TOPOLOGY topology, Transform transform, std::shared_ptr<Material> material) :
			vertexCount(vertexCount),
			topology(topology),
			transform(transform),
			material(material)
		{}

		LapisCommand(UINT vertexCount, D3D_PRIMITIVE_TOPOLOGY topology, std::shared_ptr<Material> material) :
			vertexCount(vertexCount),
			topology(topology),
			transform(Transform()),
			material(material)
		{}
	};*/
}