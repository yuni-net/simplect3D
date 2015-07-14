#ifndef si3_BonedTop_h_
#define si3_BonedTop_h_

#include "si3_top_type.h"
#include "Matrix.h"
#include "Top_pmd.h"

namespace si3
{
	class BonedTop
	{
	public:
		void init(const int top_index, const Top_pmd & top_data_);

		void renew_tops(
			DxTop * top_buffer,
			const Matrix & trans_mat,
			const Matrix & rot_mat) const;



	private:
		int top_index;
		Matrix pos_mat;
		Matrix normal_mat;
	};
}

#endif