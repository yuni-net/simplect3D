#include <si3_Quaternion.h>

namespace si3
{
	Quaternion Quaternion::operator+(const Quaternion & another) const
	{
		Quaternion result;
		result.x = x + another.x;
		result.y = y + another.y;
		result.z = z + another.z;
		result.w = w + another.w;
		return result;
	}

	Quaternion Quaternion::operator-(const Quaternion & another) const
	{
		Quaternion result;
		result.x = x - another.x;
		result.y = y - another.y;
		result.z = z - another.z;
		result.w = w - another.w;
		return result;
	}

	Quaternion Quaternion::operator*(const float value) const
	{
		Quaternion result;
		result.x = x * value;
		result.y = y * value;
		result.z = z * value;
		result.w = w * value;
		return result;
	}


}