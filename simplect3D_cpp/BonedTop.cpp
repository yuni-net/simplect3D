#include <BonedTop.h>

namespace si3
{
	void BonedTop::renew_tops(
		top_type * top_buffer,
		const matrix & trans_mat,
		const matrix & rot_mat) const
	{
		matrix new_pos_mat = pos_mat * trans_mat;
		matrix new_normal_mat = normal_mat * rot_mat;

		top_type & top = top_buffer[top_index];

		top.pos.x = new_pos_mat.x();
		top.pos.y = new_pos_mat.y();
		top.pos.z = new_pos_mat.z();

		top.normal.x = new_normal_mat.x();
		top.normal.y = new_normal_mat.y();
		top.normal.z = new_normal_mat.z();
	}

}