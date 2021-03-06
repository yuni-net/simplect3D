#pragma once

#include "si3_popular.h"
#include "si3_DisplayObject.h"
#include "si3_Leaf.h"
//#include <si3_AnimeData.h>

namespace si3
{
	class Manager;
	class AnimeData;

	class Billboard :public Leaf
	{
	public:
		Billboard();
		Billboard(const TCHAR * path);
		Billboard(const std::string & path);
		Billboard(const ::si3::ImageData & imaged);








	private:

		// @Override
		void compute_world_mat() const;

	};
}