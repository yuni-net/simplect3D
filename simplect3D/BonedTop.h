#ifndef si3_BonedTop_h_
#define si3_BonedTop_h_

#include "si3_top_type.h"
#include "matrix.h"
#include "Top_pmd.h"

namespace si3
{
	class BonedTop
	{
	public:
		void init(const int top_index, const Top_pmd & top_data_);

		void renew_tops(
			DxTop * top_buffer,
			const matrix & trans_mat,
			const matrix & rot_mat) const;



	private:
		int top_index;
		matrix pos_mat;
		matrix normal_mat;
	};
}

#endif