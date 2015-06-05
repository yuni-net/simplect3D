#include <BoneData.h>

namespace si3
{
	/***
	** ボーンと頂点の関係データを白紙に戻します。
	*/
	void BoneData::init_top_lists()
	{
		associated_tops.zerosize();
	}


	/***
	* @brief キーフレーム情報を追加します。
	*/
	void BoneData::add_key_frame(const Motion & motion)
	{
		KeyFrame key_frame;
		key_frame.frame = motion.frame_No;
		key_frame.pos.x = motion.location[0];
		key_frame.pos.y = motion.location[1];
		key_frame.pos.z = motion.location[2];
		key_frame.radian = motion.rotation[0];
		key_frame.axis.x = motion.rotation[1];
		key_frame.axis.y = motion.rotation[2];
		key_frame.axis.z = motion.rotation[3];

		key_frame_list.add(key_frame);
	}

	void BoneData::compute_trans_mat(const int now_frame)
	{
		MoveData move_data;
		if (did_bone_move(move_data, now_frame) == false)
		{
			return;
		}

		matrix rot_mat = rot_mat_of_bone(move_data);
		matrix para_mat = para_mat_of_bone(move_data);
		matrix trans_mat = rot_mat * para_mat;

		for (fw::uint top_No = 0; top_No < associated_tops.size(); ++top_No)
		{
			BonedTop & boned_top = associated_tops[top_No];
			boned_top.set_trans_mat(rot_mat, trans_mat);
		}
	}

	void BoneData::add_associated_top(const int index, Top_pmd & top, const bool is_main)
	{
		BonedTop boned_top;

		unsigned char weight = top.bone_weight;
		if (is_main == false)
		{
			weight = 100 - weight;
		}

		boned_top.init(index, top, weight);
		associated_tops.add(boned_top);
	}


	bool BoneData::did_bone_move(MoveData & move_data, const int now_frame) const
	{
		// まずキーフレームが一つもない場合は問答無用で
		if (key_frame_list.size() <= 0)
		{
			return false;
		}

		if (now_frame < key_frame_list[0].frame)
		{
			return false;
		}

		if (key_frame_list.size() == 1)
		{
			/***
			* キーフレームが一つの場合、必ず0フレーム目で設定されていると仮定する
			* 頂点情報は0フレーム目で変更し、以降は変更する必要がない
			*/

			if (now_frame > 0)
			{
				return false;
			}

			move_data.beg = &(key_frame_list[0]);
			move_data.end = &(key_frame_list[0]);
			return true;
		}

		const int final_index = key_frame_list.size() - 1;
		const int final_frame = key_frame_list[final_index].frame;
		if (now_frame > final_frame)
		{
			return false;
		}

		// キーフレームを一つずつ順番に走査して、
		// 現在の位置を探す
		fw::uint beg_index = 0;
		for (fw::uint frame_No = 1; frame_No < key_frame_list.size(); ++frame_No)
		{
			const int key_frame = key_frame_list[frame_No].frame;
			if (now_frame < key_frame)
			{
				// 現在の位置がわかったら脱出
				break;
			}

			// 現在位置は確定ではないが暫定的に進んだところにしておく
			beg_index = frame_No;
		}

		if (beg_index == final_index)
		{
			--beg_index;
		}

		move_data.beg = &(key_frame_list[beg_index]);
		move_data.end = &(key_frame_list[beg_index + 1]);

		return true;
	}


#if 0
	void BoneData::trans_and_set(D3DVECTOR & target, const float base[3], const matrix & mat)
	{
		// todo weight が全く考慮されていない
		matrix coor_mat;
		coor_mat.x(base[0]);
		coor_mat.y(base[1]);
		coor_mat.z(base[2]);
		coor_mat *= mat;
		target.x = coor_mat.x();
		target.y = coor_mat.y();
		target.z = coor_mat.z();
	}
#endif

	matrix BoneData::rot_mat_of_bone(const MoveData & move_data) const
	{
		// begとendの回転の軸が同一であるということが前提である点に注意

		const float beg_rot = move_data.beg->radian;
		const float end_rot = move_data.end->radian;
		const float ave_rot = (beg_rot + end_rot)*0.5f;

		const auto & axis = move_data.beg->axis;
		D3DXQUATERNION quate(ave_rot, axis.x, axis.y, axis.z);

		D3DXMATRIX rot_mat;
		D3DXMatrixRotationQuaternion(&rot_mat, &quate);

		return matrix(rot_mat);
	}
	matrix BoneData::para_mat_of_bone(const MoveData & move_data) const
	{
		const coor3 & beg_pos = move_data.beg->pos;
		const coor3 & end_pos = move_data.end->pos;
		coor3 ave_pos = (beg_pos + end_pos)*0.5f;
		return matrix().parallel(ave_pos.x, ave_pos.y, ave_pos.z);
	}

}