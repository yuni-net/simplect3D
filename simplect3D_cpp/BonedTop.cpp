#include <BonedTop.h>

namespace si3
{
	void BonedTop::init(const int top_index, const Top_pmd & top_data_)
	{
		this->top_index = top_index;

		pos_mat.x(top_data_.pos[0]);
		pos_mat.y(top_data_.pos[1]);
		pos_mat.z(top_data_.pos[2]);

		normal_mat.x(top_data_.normal[0]);
		normal_mat.y(top_data_.normal[1]);
		normal_mat.z(top_data_.normal[2]);
	}

	void BonedTop::renew_tops(
		DxTop * top_buffer,
		const matrix & trans_mat,
		const matrix & rot_mat) const
	{
		matrix new_pos_mat = pos_mat * trans_mat;
		matrix new_normal_mat = normal_mat * rot_mat;

		DxTop & top = top_buffer[top_index];

		top.pos.x = new_pos_mat.x();
		top.pos.y = new_pos_mat.y();
		top.pos.z = new_pos_mat.z();

		top.normal.x = new_normal_mat.x();
		top.normal.y = new_normal_mat.y();
		top.normal.z = new_normal_mat.z();
	}

}