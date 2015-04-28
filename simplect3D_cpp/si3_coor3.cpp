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

}