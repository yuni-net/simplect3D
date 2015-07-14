#include <Bone.h>

namespace si3
{
	void Bone::add_top(const int top_index, const Top_pmd & top_data_)
	{
		boned_top_list.add();
		auto & boned_top = boned_top_list.last();
		boned_top.init(top_index, top_data_);
	}
	
	void Bone::renew_tops(DxTop * top_buffer, const matrix & trans_mat, const matrix & rot_mat)
	{
		for (fw::uint boned_No = 0; boned_No < boned_top_list.size(); ++boned_No)
		{
			const auto & boned_top = boned_top_list[boned_No];
			boned_top.renew_tops(top_buffer, trans_mat, rot_mat);
		}
	}

}