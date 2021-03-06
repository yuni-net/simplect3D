#ifndef si3_sky_dome_h_
#define si3_sky_dome_h_

#include "si3_Sphere.h"
#include "si3_SkydomeData.h"

namespace si3
{
	class Skydome :public DisplayObject
	{
	public:
		Skydome();
		Skydome(const TCHAR * path);
		void load(const TCHAR * path);


		void x(float value);
		float x() const;
		void y(float value);
		float y() const;
		void z(float value);
		float z() const;

		void radius(float value);
		float radius() const;


		~Skydome();

		// @override
		void draw_no_alpha() const;

		// @override
		void draw_alpha() const;


	private:
		SkydomeData sphered;
		bool loaded;

		float x_;
		float y_;
		float z_;

		float radius_;

		void construct();
		void release();

	};
}

#endif