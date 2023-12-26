#include "DataTypes.hpp"


namespace Lapis
{

	////////////////////// Vector2

	Vector2 Lapis::Vector2::operator-() const
	{
		Vector2 res;

		res.x = -this->x;
		res.y = -this->y;

		return res;
	}
	Vector2 Vector2::operator+(const Vector2& other) const
	{
		Vector2 res;
		
		res.x = this->x + other.x;
		res.y = this->y + other.y;

		return res;
	}
	Vector2 Vector2::operator-(const Vector2& other) const
	{
		Vector2 res;

		res.x = this->x - other.x;
		res.y = this->y - other.y;

		return res;
	}

	////////////////////// Vector3

	Vector3 Lapis::Vector3::operator-() const
	{
		Vector3 res;

		res.x = -this->x;
		res.y = -this->y;
		res.z = -this->z;

		return res;
	}

	////////////////////// Vector4

	Vector4 Lapis::Vector4::operator+(const Vector4& other) const
	{
		Vector4 res;

		res.x = this->x + other.x;
		res.y = this->y + other.y;
		res.z = this->z + other.z;
		res.w = this->w + other.w;

		return res;
	}

}