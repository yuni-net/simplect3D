#ifndef si3_Bone_h_
#define si3_Bone_h_

#include "si3_DxTop.h"
#include "si3_Matrix.h"
#include "si3_BonedTop.h"
#include "si3_Top_pmd.h"
#include "si3_MotionData.h"

namespace si3
{
	class Bone
	{
	public:
		void add_top(const int top_index, const Top_pmd & top_data_);

		void renew_tops(DxTop * top_buffer, const Matrix & trans_mat, const Matrix & rot_mat);



	private:
		fw::Array<BonedTop> boned_top_list;
	};
}

#endif