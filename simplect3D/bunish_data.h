#ifndef si3_bunish_data_h_
#define si3_bunish_data_h_

#include "utility.h"
#include "Manager.h"
#include "matrix.h"

namespace si3
{
	class bunish_data
	{
	public:
		void set_local_pos(uint index, const Coor3 & dust);
		void set_first_pos(const Coor3 & value);
		void set_bunish_vec(const Coor3 & value);
		void set_rot_speed(const Coor3 & value);
		void bunish(const si3::Manager & si3m);
		D3DVECTOR compute_local_pos(uint index) const;

		// 降順にソートされる点に注意
		bool operator<(const bunish_data & another) const;


		bunish_data();
	private:
		Coor3 bunish_vec;
		Coor3 rot_speed;

		Coor3 pos;
		Coor3 radian;
		matrix world_mat;
		float z;

		Coor3 local_pos_list[3];


		void compute_world_mat();
		void compute_z(const si3::Manager & si3m);
	};

}

#endif