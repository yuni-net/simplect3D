#pragma once

#include "si3_Vector.h"

namespace si3
{
	class UniVec3
	{
	public:
		UniVec3();
		UniVec3(const Vector3 & another);
		void init(const Vector3 & another);
		Vector3 operator*(const float val) const;
		float x() const;
		float y() const;
		float z() const;



	private:
		float x_;
		float y_;
		float z_;
	};
}