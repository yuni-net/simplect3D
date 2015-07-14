#ifndef si3_BoneBranch_h_
#define si3_BoneBranch_h_

#include "si3_top_type.h"
#include "MotionData.h"
#include "Matrix.h"
#include "Bone.h"

namespace si3
{
	class BoneBranch
	{
	public:
		std::string bone_name;

		void init(const int bone_No, const float bone_pos[3], const std::string & bone_name);

		void add_child(BoneBranch & child);

		void add_top(const int top_index, const Top_pmd & top_data_);

		void renew_tops(
			DxTop * top_buffer,
			MotionData & motion_data,
			const Matrix & parent_trans_mat,
			const Matrix & parent_rot_mat,
			bool todo_renew);



	private:
		int bone_No;
		Coor3 bone_pos;
		Bone bone;
		fw::Array<BoneBranch *> child_list;
	};

}

#endif