#pragma once

#include "popular.h"
#include "display_object.h"
#include "leaf.h"
//#include <anime_data.h>

namespace si3
{
	class manager;
	class anime_data;

	class billboard :public leaf
	{
	public:
		billboard();
		billboard(const manager & manageri, const TCHAR * path);
		billboard(const manager & manageri, const std::string & path);
		billboard(const manager & manageri, const ::si3::image_data & imaged);








	private:

		// @Override
		void compute_world_mat() const;

	};
}