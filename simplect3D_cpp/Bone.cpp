#include <Bone.h>

namespace si3
{
	void Bone::renew_tops(top_type * top_buffer, const matrix & trans_mat, const matrix & rot_mat)
	{
		for (fw::uint boned_No = 0; boned_No < boned_top_list.size(); ++boned_No)
		{
			const auto & boned_top = boned_top_list[boned_No];
			boned_top.renew_tops(top_buffer, trans_mat, rot_mat);
		}
	}

}