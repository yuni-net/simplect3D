#ifndef si3_BoneBranch_h_
#define si3_BoneBranch_h_

#include "si3_top_type.h"
#include "MotionData.h"
#include "matrix.h"
#include "Bone.h"

namespace si3
{
	class BoneBranch
	{
	public:
		void renew_tops(
			top_type * top_buffer,
			MotionData & motion_data,
			const matrix & parent_trans_mat,
			const matrix & parent_rot_mat,
			bool todo_renew);



	private:
		Bone bone;
		fw::vector<BoneBranch> child_list;
	};

}

#endif