#ifndef si3_sky_dome_h_
#define si3_sky_dome_h_

#include "sphere.h"
#include "sky_dome_data.h"

namespace si3
{
	class Skydome :public DisplayObject
	{
	public:
		Skydome();
		Skydome(const Manager & manageri, const TCHAR * path);
		void load(const Manager & manageri, const TCHAR * path);


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
		const Manager * managerp;
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