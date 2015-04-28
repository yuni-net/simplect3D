#include <utility.h>

namespace si3
{
	coor3 normalize_vector(const coor3 & base)
	{
		float length2 = base.x*base.x + base.y*base.y + base.z*base.z;
		if (abs(length2 - 1.0f) < 0.000000001f)
		{
			// length2 = 1
			// length = 1
			return base;
		}

		float length = pow(length2, 0.5f);

		coor3 normal;
		normal.x = base.x / length;
		normal.y = base.y / length;
		normal.z = base.z / length;

		return normal;
	}

}