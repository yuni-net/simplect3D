#include <si3_UniVec3.h>

namespace si3
{
	UniVec3::UniVec3()
	{
		init(Vector3(1.0f, 0.0f, 0.0f));
	}
	UniVec3::UniVec3(const Vector3 & another)
	{
		init(another);
	}
	void UniVec3::init(const Vector3 & another)
	{
		const float percent = 1.0f/another.length();
		x_ = another.x()*percent;
		y_ = another.y()*percent;
		z_ = another.z()*percent;
	}

	Vector3 UniVec3::operator*(const float val) const
	{
		return Vector3(x()*val, y()*val, z()*val);
	}

	float UniVec3::x() const
	{
		return x_;
	}
	float UniVec3::y() const
	{
		return y_;
	}
	float UniVec3::z() const
	{
		return z_;
	}

}