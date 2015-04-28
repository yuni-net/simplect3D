#include <utility.h>
#include <popular.h>

namespace si3
{
	void vector_to_unit_vector(const D3DVECTOR & from, D3DVECTOR & to)
	{
		float length = sqrtf(from.x*from.x + from.y*from.y + from.z*from.z);

		to.x = from.x / length;
		to.y = from.y / length;
		to.z = from.z / length;
	}

	uchar round_uchar(float fVal)
	{
		if (fVal < 0.00000000000f)
		{
			return static_cast<uchar>(fVal - 0.55555555555f);
		}

		return static_cast<uchar>(fVal + 0.555555555555f);
	}

}