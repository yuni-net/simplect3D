#ifndef si3_Bone_h_
#define si3_Bone_h_

#include "si3_top_type.h"
#include "matrix.h"
#include "BonedTop.h"

namespace si3
{
	class Bone
	{
	public:
		void renew_tops(top_type * top_buffer, const matrix & trans_mat, const matrix & rot_mat);



	private:
		fw::vector<BonedTop> boned_top_list;
	};
}

#endif