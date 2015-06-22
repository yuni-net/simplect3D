#ifndef si3_Quaternion_h_
#define si3_Quaternion_h_

namespace si3
{
	struct Quaternion
	{
		float x;
		float y;
		float z;
		float w;

		Quaternion operator+(const Quaternion & another) const;
		Quaternion operator-(const Quaternion & another) const;
		Quaternion operator*(const float value) const;
	};
}

#endif