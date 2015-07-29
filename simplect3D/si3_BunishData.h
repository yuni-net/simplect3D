#ifndef si3_bunish_data_h_
#define si3_bunish_data_h_

#include "si3_utility.h"
#include "si3_Manager.h"
#include "si3_Matrix.h"

namespace si3
{
	/***
	モデルをバラバラに分解する表現の、モデルデータ部分を管理するクラス。
	*/
	class BunishData
	{
	public:
		void set_local_pos(uint index, const Coor3 & dust);
		void set_first_pos(const Coor3 & value);
		void set_bunish_vec(const Coor3 & value);
		void set_rot_speed(const Coor3 & value);
		void bunish();
		D3DVECTOR compute_local_pos(uint index) const;

		// 降順にソートされる点に注意
		bool operator<(const BunishData & another) const;


		BunishData();
	private:
		Coor3 bunish_vec;
		Coor3 rot_speed;

		Coor3 pos;
		Coor3 radian;
		Matrix world_mat;
		float z;

		Coor3 local_pos_list[3];


		void compute_world_mat();
		void compute_z();
	};

}

#endif