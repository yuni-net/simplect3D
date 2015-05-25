#ifndef si3_sky_dome_h_
#define si3_sky_dome_h_

#include "sphere.h"

namespace si3
{
	class sky_dome :public display_object
	{
	public:
		sky_dome();
		sky_dome(const manager & manageri, const TCHAR * path);
		void load(const manager & manageri, const TCHAR * path);


		void x(float value);
		float x() const;
		void y(float value);
		float y() const;
		void z(float value);
		float z() const;

		void radius(float value);
		float radius() const;


		~sky_dome();

		// @override
		void draw_no_alpha() const;

		// @override
		void draw_alpha() const;


	private:
		const manager * managerp;;
		sphere_data sphered;
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