#include <si3_BoneBranch.h>

namespace si3
{
	void BoneBranch::init(const int bone_No, const float bone_pos[3], const std::string & bone_name)
	{
		this->bone_No = bone_No;

		this->bone_pos.x = bone_pos[0];
		this->bone_pos.y = bone_pos[1];
		this->bone_pos.z = bone_pos[2];

		this->bone_name = bone_name;
	}


	void BoneBranch::add_child(BoneBranch & child)
	{
		child_list.add(&child);
	}

	void BoneBranch::add_top(const int top_index, const Top_pmd & top_data_)
	{
		bone.add_top(top_index, top_data_);
	}

	void BoneBranch::renew_tops(
		DxTop * top_buffer,
		MotionData & motion_data,
		const Matrix & parent_trans_mat,
		const Matrix & parent_rot_mat,
		bool todo_renew)
	{
		Matrix trans_mat = parent_trans_mat;
		Matrix rot_mat = parent_rot_mat;

		Matrix my_trans_mat;
		Matrix my_rot_mat;

		// debug
		if (bone_name == "右ひじ" || bone_name=="左ひじ")
		{
			int a = 0;
		}

		const bool todo_trans = motion_data.compute_trans_mat(my_trans_mat, my_rot_mat, bone_pos, bone_No);
		if (todo_trans)
		{
			todo_renew = true;
			trans_mat = parent_trans_mat * my_trans_mat;
			rot_mat = parent_rot_mat * my_rot_mat;
		}

		if (todo_renew)
		{
			bone.renew_tops(top_buffer, trans_mat, rot_mat);
		}

		for (fw::uint child_No = 0; child_No < child_list.size(); ++child_No)
		{
			BoneBranch & child = *(child_list[child_No]);
			child.renew_tops(top_buffer, motion_data, trans_mat, rot_mat, todo_renew);
		}
	}

}