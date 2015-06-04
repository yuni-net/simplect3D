#include <utility.h>

namespace si3
{
	coor3 & coor3::operator+=(const coor3 & another)
	{
		x += another.x;
		y += another.y;
		z += another.z;

		return *this;
	}

	coor3 coor3::operator+(const coor3 & another) const
	{
		coor3 result;
		result.x = this->x + another.x;
		result.y = this->y + another.y;
		result.z = this->z + another.z;
		return result;
	}

	coor3 coor3::operator*(const float magni) const
	{
		coor3 result;
		result.x = this->x * magni;
		result.y = this->y * magni;
		result.z = this->z * magni;
		return result;
	}

}