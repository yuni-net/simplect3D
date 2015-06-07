#include <BoneBranch.h>

namespace si3
{
	void BoneBranch::renew_tops(
		top_type * top_buffer,
		MotionData & motion_data,
		const matrix & parent_trans_mat,
		const matrix & parent_rot_mat,
		bool todo_renew)
	{
		matrix trans_mat = parent_trans_mat;
		matrix rot_mat = parent_rot_mat;

		if (bone.todo_renew(motion_data))
		{
			todo_renew = true;
			trans_mat = parent_trans_mat * bone.compute_trans_mat(motion_data);
			rot_mat = parent_rot_mat * bone.compute_rot_mat(motion_data);
			bone.renew_tops(top_buffer, trans_mat, rot_mat);
		}

		for (fw::uint child_No = 0; child_No < child_list.size(); ++child_No)
		{
			auto & child = child_list[child_No];
			child.renew_tops(top_buffer, motion_data, trans_mat, rot_mat, todo_renew);
		}
	}

}