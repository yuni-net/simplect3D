#include <si3_utility.h>

namespace si3
{
	bool Coor3::operator !=(const Coor3 & another) const
	{
		if (x != another.x) return true;
		if (y != another.y) return true;
		if (z != another.z) return true;

		return false;
	}

	Coor3 & Coor3::operator+=(const Coor3 & another)
	{
		x += another.x;
		y += another.y;
		z += another.z;

		return *this;
	}

	Coor3 Coor3::operator+(const Coor3 & another) const
	{
		Coor3 result;
		result.x = this->x + another.x;
		result.y = this->y + another.y;
		result.z = this->z + another.z;
		return result;
	}

	Coor3 Coor3::operator-(const Coor3 & another) const
	{
		Coor3 result;
		result.x = this->x - another.x;
		result.y = this->y - another.y;
		result.z = this->z - another.z;
		return result;
	}

	Coor3 Coor3::operator*(const float magni) const
	{
		Coor3 result;
		result.x = this->x * magni;
		result.y = this->y * magni;
		result.z = this->z * magni;
		return result;
	}

}