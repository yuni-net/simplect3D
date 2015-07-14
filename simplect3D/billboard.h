#pragma once

#include "popular.h"
#include "display_object.h"
#include "Leaf.h"
//#include <anime_data.h>

namespace si3
{
	class Manager;
	class AnimeData;

	class Billboard :public Leaf
	{
	public:
		Billboard();
		Billboard(const Manager & manageri, const TCHAR * path);
		Billboard(const Manager & manageri, const std::string & path);
		Billboard(const Manager & manageri, const ::si3::ImageData & imaged);








	private:

		// @Override
		void compute_world_mat() const;

	};
}