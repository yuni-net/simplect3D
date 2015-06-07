#ifndef si3_Bone_h_
#define si3_Bone_h_

#include "si3_top_type.h"
#include "matrix.h"
#include "BonedTop.h"
#include "Top_pmd.h"
#include "MotionData.h"

namespace si3
{
	class Bone
	{
	public:
		void add_top(const int top_index, const Top_pmd & top_data_);

		void renew_tops(top_type * top_buffer, const matrix & trans_mat, const matrix & rot_mat);



	private:
		fw::vector<BonedTop> boned_top_list;
	};
}

#endif