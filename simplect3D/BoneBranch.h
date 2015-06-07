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
		void init(const int bone_No, const float bone_pos[3]);

		void add_child(BoneBranch & child);

		void add_top(const int top_index, const Top_pmd & top_data_);

		void renew_tops(
			top_type * top_buffer,
			MotionData & motion_data,
			const matrix & parent_trans_mat,
			const matrix & parent_rot_mat,
			bool todo_renew);



	private:
		int bone_No;
		coor3 bone_pos;
		Bone bone;
		fw::vector<BoneBranch *> child_list;
	};

}

#endif