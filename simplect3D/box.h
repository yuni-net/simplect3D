#if 1

#pragma once

#include "display_object.h"
#include "popular.h"
#include "box_data.h"

namespace si3
{
	class box :public display_object
	{
	public:
		void set_box_data(const box_data & boxd);

		void x(float value);
		float x() const;

		void y(float value);
		float y() const;

		void z(float value);
		float z() const;

		float rot_x() const;
		void rot_x(float value);

		float rot_y() const;
		void rot_y(float value);

		float rot_z() const;
		void rot_z(float value);


		float red() const;
		void red(float value);

		float green() const;
		void green(float value);

		float blue() const;
		void blue(float value);

		float alpha() const;
		void alpha(float value);

		void draw_no_alpha() const;
		void draw_alpha() const;




	private:
		float x_;
		float y_;
		float z_;
		float red_;
		float green_;
		float blue_;
		float alpha_;

		float rot_x_;
		float rot_y_;
		float rot_z_;

		const box_data * boxd;
	};
}

#endif