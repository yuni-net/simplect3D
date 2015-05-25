#if 1

#pragma once

#include "display_object.h"
#include "popular.h"
#include "sphere_data.h"

namespace si3
{
	class manager;

	class sphere :public display_object
	{
	public:
		sphere();
		sphere(const manager & si3m, const char * path, float smoothness, float radius);
		void create(const manager & si3m, const char * path, float smoothness, float radius);

		void x(float value);
		float x() const;

		void y(float value);
		float y() const;

		void z(float value);
		float z() const;

		float radius() const;

#if 1
		void red(float value);
		float red() const;

		void green(float value);
		float green() const;

		void blue(float value);
		float blue() const;

		void alpha(float value);
		float alpha() const;

#endif

		void rot_x(float value);
		float rot_x() const;

		void rot_y(float value);
		float rot_y() const;

		void rot_z(float value);
		float rot_z() const;



		void draw_no_alpha() const;
		void draw_alpha() const;

		~sphere();

	private:
		LPDIRECT3DDEVICE9 device;
		float x_;
		float y_;
		float z_;
		float radius_;

		float red_;
		float green_;
		float blue_;
		float alpha_;

		float rot_x_;
		float rot_y_;
		float rot_z_;

		sphere_data * sphered;
		bool ownership;


		void construct();
	};
}

#endif