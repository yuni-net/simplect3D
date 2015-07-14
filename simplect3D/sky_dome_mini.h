#ifndef sky_dome_mini_h_
#define sky_dome_mini_h_

#include "popular.h"
#include "display_object.h"
#include "rot_mode.h"
#include "sphere_data.h"
#include "sky_dome_data.h"

namespace si3
{
	class Manager;


	class sky_dome_mini :public display_object
	{
	public:
		sky_dome_mini();
		sky_dome_mini(const Manager & manageri, const TCHAR * path);
		void load(const Manager & manageri, const TCHAR * path);




		~sky_dome_mini();

		// @override
		void draw_no_alpha() const;

		// @override
		void draw_alpha() const;


	private:
		const Manager * managerp;;
		sky_dome_data sphered;
		bool loaded;

		void construct();
		void release();

	};
}

#endif