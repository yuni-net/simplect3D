#pragma once

#include "si3_coor3.h"

namespace si3
{
	class Vector3
	{
	public:
		Vector3();
		Vector3(const Vector3 & another);
		Vector3(const Coor3 & pos);
		Vector3(const float x, const float y, const float z);
		float x() const;
		float y() const;
		float z() const;
		float length2() const;
		float length() const;
		void x(const float val);
		void y(const float val);
		void z(const float val);



	private:
		float x_;
		float y_;
		float z_;
	};
}