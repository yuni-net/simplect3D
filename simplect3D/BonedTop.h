#ifndef si3_BonedTop_h_
#define si3_BonedTop_h_

#include "si3_top_type.h"
#include "matrix.h"

namespace si3
{
	class BonedTop
	{
	public:
		void renew_tops(
			top_type * top_buffer,
			const matrix & trans_mat,
			const matrix & rot_mat) const;



	private:
		int top_index;
		matrix pos_mat;
		matrix normal_mat;
	};
}

#endif