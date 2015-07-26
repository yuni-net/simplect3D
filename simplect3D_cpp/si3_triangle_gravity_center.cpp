#include <si3_popular.h>
#include <si3_coor3.h>

namespace si3
{
	Coor3 triangle_gravity_center(const D3DVECTOR & pos0, const D3DVECTOR & pos1, const D3DVECTOR & pos2)
	{
		Coor3 center;

		center.x = (pos0.x + pos1.x + pos2.x) / 3.0f;
		center.y = (pos0.y + pos1.y + pos2.y) / 3.0f;
		center.z = (pos0.z + pos1.z + pos2.z) / 3.0f;

		return center;
	}
}