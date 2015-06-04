#ifndef si3_coor3_h_
#define si3_coor3_h_

namespace si3
{
	struct coor3
	{
		float x;
		float y;
		float z;

		coor3 & operator+=(const coor3 & another);
		coor3 operator+(const coor3 & another) const;
		coor3 operator*(const float magni) const;
	};

}

#endif