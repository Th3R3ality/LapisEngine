#include "DataTypes.hpp"


namespace Lapis
{
	Vector4 Lapis::Vector4::operator+(const Vector4& other)
	{
		Vector4 res;

		res.x = this->x + other.x;
		res.y = this->y + other.y;
		res.z = this->z + other.z;
		res.w = this->w + other.w;

		return res;
	}
}