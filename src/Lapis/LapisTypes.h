#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <memory>
#include <string>
#include <iostream>

namespace Lapis
{
	enum class Shape
	{
		Icosahedron,
		Cube,
		Triangle,
		Plane
	};

	struct Color
	{
		union
		{
			struct
			{
				float r;
				float g;
				float b;
				float a;
			};
			DXGI_RGBA rgba;
		};

		constexpr Color() :
			rgba({ 0,0,0,0 })
		{}

		constexpr Color(float f) :
			rgba({ f,f,f,f })
		{}

		constexpr Color(int i)
		{
			float f = static_cast<float>(i);
			rgba = { f,f,f,f };
		}

		constexpr Color(float r, float g, float b, float a) :
			rgba({ r,g,b,a })
		{}

		Color(const char* hex) :
			rgba(hex2rgba(hex))
		{}

		operator DXGI_RGBA();

		static Color hex2rgba(const char* hex);
	};
	//using Color = DXGI_RGBA;

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
		union {
			struct {
				float x, y, z;
			};
			struct {
				float pitch, yaw, roll;
			};
			struct {
				float r, g, b;
			};
		};

		Vec3 operator-() const;
		Vec3 operator+(const Vec3& other) const;
		Vec3 operator*(const float& scalar) const;

		Vec3& operator+=(const Vec3& other);
		Vec3& operator-=(const Vec3& other);

		friend std::ostream& operator<<(std::ostream& os, const Vec3& v3)
		{
			os << v3.x << '/' << v3.y << '/' << v3.z;
			return os;
		}

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

		void Normalize();

	};
	using Vector3 = Vec3;


	struct Vec4
	{
		union
		{
			float i[4];
			struct
			{
				float x, y;
				union
				{
					float z;
					float width;
				};
				union
				{
					float w;
					float height;
				};
			};
		};

		Vec4 operator+(const Vec4& other) const;
		Vec4& operator=(const RECT& other);

		Vec4& operator=(const DirectX::XMVECTOR& other);
		operator DirectX::XMVECTOR();

		friend std::ostream& operator<<(std::ostream& os, const Vec4& v3)
		{
			os << v3.x << "/" << v3.y << "/" << v3.z << "/" << v3.w;
			return os;
		}

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

		Vec4(DirectX::XMVECTOR& v4) :
			x(v4.m128_f32[0]), y(v4.m128_f32[1]), z(v4.m128_f32[2]), w(v4.m128_f32[3])
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
			Color color;
			Color col;
		};
		Vec2 uv;
		Vec3 normal;

		Vertex(Vec3 pos, Color col, Vec2 uv, Vec3 normal) :
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
		
		Vec3 _Forward();
		Vec3 _Right();
		Vec3 _Up();
		
		Vec3 Forward();
		Vec3 Right();
		Vec3 Up();
	};

	struct mat4x4
	{
		union
		{
			Vec4 r[4];
			float m[4][4];
			struct
			{
				float _00, _10, _20, _30;
				float _01, _11, _21, _31;
				float _02, _12, _22, _32;
				float _03, _13, _23, _33;
			};
		};

		mat4x4(const mat4x4&) = default;

		mat4x4() :
			r{ 0,0,0,0 }
		{}
		mat4x4(Vec4 r0, Vec4 r1, Vec4 r2, Vec4 r3) :
			r{ r0, r1, r2, r3 }
		{}

		mat4x4(DirectX::XMMATRIX mat) :
			r{ mat.r[0], mat.r[0], mat.r[0], mat.r[0] }
		{}
		mat4x4(DirectX::XMMATRIX& mat) :
			r{ mat.r[0], mat.r[0], mat.r[0], mat.r[0] }
		{}

		mat4x4& operator=(const DirectX::XMMATRIX& other);
		operator DirectX::XMMATRIX();

		friend std::ostream& operator<<(std::ostream& os, const mat4x4& mat)
		{
			os << mat.r[0] << " | " << mat.r[1] << " | " << mat.r[2] << " | " << mat.r[3];
			return os;
		}

		static const mat4x4 Identity;
	};

	struct Quad
	{
		Vec3 a, b, c, d;
	};


	struct Material
	{

	};
}