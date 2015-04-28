#if 0

#pragma once

#include "display_object.h"
#include "popular.h"
#include "box_data.h"

namespace si3
{
	class box :public display_object
	{
	public:
		void x(float value);
		float x() const;

		void y(float value);
		float y() const;

		void z(float value);
		float z() const;

		void radius(float value);
		float radius() const;

#if 0	// ç°ÇÕÇ‹Çæñ¢ëŒâû
		void red(float value);
		float red() const;

		void green(float value);
		float green() const;

		void blue(float value);
		float blue() const;
#endif


		void draw_no_alpha() const;
		void draw_alpha() const;




	private:
		float x_;
		float y_;
		float z_;
		float radius_;
		float red_;
		float green_;
		float blue_;

		const box_data * boxd;
		bool ownership;
	};
}

#endif