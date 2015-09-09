#ifndef si3_coor3_h_
#define si3_coor3_h_

#include "si3_popular.h"

namespace si3
{
	class Coor3
	{
	public:
		float x;
		float y;
		float z;

		Coor3();
		Coor3(const D3DXVECTOR3 & dxcoor);
		Coor3(const float x, const float y, const float z);
		bool operator !=(const Coor3 & another) const;
		Coor3 & operator+=(const Coor3 & another);
		Coor3 operator+(const Coor3 & another) const;
		Coor3 operator-(const Coor3 & another) const;
		Coor3 operator*(const float magni) const;
		float length2() const;
		float length() const;
		void normalize();
		D3DXVECTOR3 dxvec3() const;

		static float dot(const si3::Coor3 & one, const si3::Coor3 & ano);
		static si3::Coor3 cross(const si3::Coor3 one, const si3::Coor3 & ano);
	};

}

#endif