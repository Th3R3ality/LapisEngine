#include "LapisTypes.h"
#include "GlobalDefines.h"

namespace Lapis
{

	////////////////////// Vec2

	Vec2 Lapis::Vec2::operator-() const
	{
		Vec2 res;

		res.x = -this->x;
		res.y = -this->y;

		return res;
	}
	Vec2 Vec2::operator+(const Vec2& other) const
	{
		Vec2 res;

		res.x = this->x + other.x;
		res.y = this->y + other.y;

		return res;
	}
	Vec2 Vec2::operator-(const Vec2& other) const
	{
		Vec2 res;

		res.x = this->x - other.x;
		res.y = this->y - other.y;

		return res;
	}
	Vec2 Vec2::operator*(const float& mul) const
	{
		Vec2 res;

		res.x = x * mul;
		res.y = y * mul;

		return res;
	}

	////////////////////// Vec3

	Vec3 const Vec3::right(1, 0, 0);
#if USE_Z_UP == 0
	Vec3 const Vec3::up(0, 1, 0);
	Vec3 const Vec3::forward(0, 0, 1);
#elif USE_Z_UP == 1
	Vec3 const Vec3::up(0, 0, 1);
	Vec3 const Vec3::forward(0, 1, 0);
#endif

	Vec3 Lapis::Vec3::operator-() const
	{
		Vec3 res;

		res.x = -this->x;
		res.y = -this->y;
		res.z = -this->z;

		return res;
	}

	Vec3 Vec3::operator+(const Vec3& other) const
	{
		Vec3 res;

		res.x = x + other.x;
		res.y = y + other.y;
		res.z = z + other.z;

		return res;
	}

	Vec3 Vec3::operator*(const float& scalar) const
	{
		Vec3 res;

		res.x = x * scalar;
		res.y = y * scalar;
		res.z = z * scalar;

		return res;
	}

	Vec3& Vec3::operator+=(const Vec3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;

		return *this;
	}

	Vec3& Vec3::operator-=(const Vec3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;

		return *this;
	}

	////////////////////// Vec4

	Vec4 Lapis::Vec4::operator+(const Vec4& other) const
	{
		Vec4 res;

		res.x = this->x + other.x;
		res.y = this->y + other.y;
		res.z = this->z + other.z;
		res.w = this->w + other.w;

		return res;
	}

}