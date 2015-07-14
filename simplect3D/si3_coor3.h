#ifndef si3_coor3_h_
#define si3_coor3_h_

namespace si3
{
	struct Coor3
	{
		float x;
		float y;
		float z;

		bool operator !=(const Coor3 & another) const;
		Coor3 & operator+=(const Coor3 & another);
		Coor3 operator+(const Coor3 & another) const;
		Coor3 operator-(const Coor3 & another) const;
		Coor3 operator*(const float magni) const;
	};

}

#endif