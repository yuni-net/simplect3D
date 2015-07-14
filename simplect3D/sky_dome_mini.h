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


	class Skydome_mini :public DisplayObject
	{
	public:
		Skydome_mini();
		Skydome_mini(const Manager & manageri, const TCHAR * path);
		void load(const Manager & manageri, const TCHAR * path);




		~Skydome_mini();

		// @override
		void draw_no_alpha() const;

		// @override
		void draw_alpha() const;


	private:
		const Manager * managerp;;
		SkydomeData sphered;
		bool loaded;

		void construct();
		void release();

	};
}

#endif