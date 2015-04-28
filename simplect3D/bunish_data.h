#ifndef si3_bunish_data_h_
#define si3_bunish_data_h_

#include "utility.h"
#include "manager.h"
#include "matrix.h"

namespace si3
{
	class bunish_data
	{
	public:
		void set_local_pos(uint index, const coor3 & dust);
		void set_first_pos(const coor3 & value);
		void set_bunish_vec(const coor3 & value);
		void set_rot_speed(const coor3 & value);
		void bunish(const si3::manager & si3m);
		D3DVECTOR compute_local_pos(uint index) const;

		// 降順にソートされる点に注意
		bool operator<(const bunish_data & another) const;


		bunish_data();
	private:
		coor3 bunish_vec;
		coor3 rot_speed;

		coor3 pos;
		coor3 radian;
		matrix world_mat;
		float z;

		coor3 local_pos_list[3];


		void compute_world_mat();
		void compute_z(const si3::manager & si3m);
	};

}

#endif