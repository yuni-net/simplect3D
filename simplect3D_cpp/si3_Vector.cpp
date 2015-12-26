#include <si3_Vector.h>
#include <cmath>

namespace si3
{
	Vector3::Vector3()
	{
		// Nothing
	}

	Vector3::Vector3(const Vector3 & another)
	{
		x(another.x());
		y(another.y());
		z(another.z());
	}

	Vector3::Vector3(const Coor3 & pos)
	{
		x(pos.x);
		y(pos.y);
		z(pos.z);
	}

	Vector3::Vector3(const float x, const float y, const float z)
	{
		this->x(x);
		this->y(y);
		this->z(z);
	}

	float Vector3::x() const
	{
		return x_;
	}
	float Vector3::y() const
	{
		return y_;
	}
	float Vector3::z() const
	{
		return z_;
	}

	float Vector3::length2() const
	{
		return x()*x() + y()*y() + z()*z();
	}
	float Vector3::length() const
	{
		return sqrtf(length2());
	}

	void Vector3::x(const float val)
	{
		x_ = val;
	}
	void Vector3::y(const float val)
	{
		y_ = val;
	}
	void Vector3::z(const float val)
	{
		z_ = val;
	}


}