#include <algorithm>
#include <BoneMotion.h>

namespace si3
{


	/***
	* @brief キーフレーム情報を追加します。
	*/
	void BoneMotion::add_key_frame(const Motion & motion)
	{
		KeyFrame key_frame;
		key_frame.frame = motion.frame_No;
		key_frame.pos.x = motion.location[0];
		key_frame.pos.y = motion.location[1];
		key_frame.pos.z = motion.location[2];
		key_frame.quate.x = motion.rotation[0];
		key_frame.quate.y = motion.rotation[1];
		key_frame.quate.z = motion.rotation[2];
		key_frame.quate.w = motion.rotation[3];

		key_frame_list.push_back(key_frame);
	}

	void BoneMotion::finish_add_key_frame()
	{
		std::sort(key_frame_list.begin(), key_frame_list.end());
	}

	/***
	* @brief 現在のフレームのこのボーンの座標変換行列を計算します。
	* @param
	*  [out]trans_mat: 変換行列がここに格納されます
	*  [out]rot_mat: 変換行列の回転成分のみがここに格納されます
	*  now_frame: 現在のフレーム指定します
	* @return
	*  座標を変換する必要がある場合はtrueを、そもそも変換する必要が無い場合はfalseを返します。
	*  例えば前のフレームから変化が無ければ座標を再度変換する必要は無いわけです。
	*  ただし、必ず１フレームずつ順番にアニメーションすることが前提になっています。
	*  逆再生や、任意のフレームにテレポートした場合は当然変換する必要があるでしょう。
	*/
	bool BoneMotion::compute_trans_mat(
		Matrix & trans_mat,
		Matrix & rot_mat,
		const Coor3 & bone_pos,
		const int now_frame)
	{
		MoveData move_data;
		bool todo_trans;
		bool todo_judge_move;

		compute_move_data(move_data, todo_trans, todo_judge_move, now_frame);

		if (todo_trans == false)
		{
			return false;
		}

		if (todo_judge_move)
		{
			if (did_bone_move(move_data) == false)
			{
				return false;
			}
		}

		const float percent = compute_percent(move_data, now_frame);

		D3DXQUATERNION quate;
		rot_mat = rot_mat_of_bone(move_data, percent, quate);

		trans_mat = trans_mat_of_bone(move_data, percent, bone_pos, quate);

		return true;
	}



	void BoneMotion::compute_move_data(
		MoveData & move_data,
		bool & todo_trans,
		bool & todo_judge_move,
		const int now_frame)
	{
		// まずキーフレームが一つもない場合は問答無用で
		if (key_frame_list.size() <= 0)
		{
			todo_trans = false;
			return;
		}

		if (now_frame < key_frame_list[0].frame)
		{
			todo_trans = false;
			return;
		}

		if (key_frame_list.size() == 1)
		{
			/***
			* キーフレームが一つの場合、必ず0フレーム目で設定されていると仮定する
			* 頂点情報は0フレーム目で変更し、以降は変更する必要がない
			*/

			if (now_frame > 0)
			{
				todo_trans = false;
				return;
			}

			move_data.beg = &(key_frame_list[0]);
			move_data.end = &(key_frame_list[0]);
			todo_trans = true;
			todo_judge_move = false;
			return;
		}

		const int final_index = key_frame_list.size() - 1;
		const int final_frame = key_frame_list[final_index].frame;
		if (now_frame > final_frame)
		{
			todo_trans = false;
			return;
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

		todo_trans = true;
		todo_judge_move = true;
	}

	/***
	* move_dataのbegとendが異なる値であるかどうかを判定します。
	* 異なる場合はtrueを、同じである場合はfalseを返します。
	*/
	bool BoneMotion::did_bone_move(MoveData & move_data) const
	{
		const KeyFrame & beg = *(move_data.beg);
		const KeyFrame & end = *(move_data.end);

		if (beg.pos != end.pos)
		{
			return true;
		}

		if (beg.quate.x != end.quate.x ||
			beg.quate.y != end.quate.y || 
			beg.quate.z != end.quate.z || 
			beg.quate.w != end.quate.w )
		{
			return true;
		}

		return false;
	}

	float BoneMotion::compute_percent(MoveData & move_data, const int now_frame) const
	{
		const KeyFrame & beg = *(move_data.beg);
		const KeyFrame & end = *(move_data.end);

		const int rel_frame = now_frame - beg.frame;

		if (rel_frame == 0)
		{
			return 0.0f;
		}

		const int gap_frame = end.frame - beg.frame;

		return static_cast<float>(rel_frame) / gap_frame;
	}




	/***
	* @brief move_dataに基づき現在のフレームの回転行列を計算してそれを返す。
	* @param
	*  move_data: MoveDataのインスタンスを指定します。
	*  percent: begフレームとendフレームの間のどこに居るのかを割合で指定します(0.0f-1.0f)。
	*  [out]quate: 計算の結果得られたクォータニオンがここに格納されます。
	* @return 現在のフレームの回転行列を返す。
	*/
	Matrix BoneMotion::rot_mat_of_bone(const MoveData & move_data, const float percent, D3DXQUATERNION & quate) const
	{
		const Quaternion & quate_beg = move_data.beg->quate;
		const Quaternion & quate_end = move_data.end->quate;
		Quaternion quate_now = quate_beg + (quate_end - quate_beg)*percent;
		
		D3DXQUATERNION quate_dx(quate_now.x, quate_now.y, quate_now.z, quate_now.w);
		quate = quate_dx;

		D3DXMATRIX rot_mat;
		D3DXMatrixRotationQuaternion(&rot_mat, &quate);

		return Matrix(rot_mat);
	}

#if 0
	Matrix BoneMotion::para_mat_of_bone(const MoveData & move_data, const float percent) const
	{
		const Coor3 & beg_pos = move_data.beg->pos;
		const Coor3 & end_pos = move_data.end->pos;
		Coor3 now_pos = (beg_pos + end_pos)*percent;
		return Matrix().parallel(now_pos.x, now_pos.y, now_pos.z);
	}
#endif

	/***
	* @brief move_dataに基づき現在のフレームの変換行列を計算してそれを返す。
	* @param
	*  move_data: MoveDataのインスタンスを指定します。
	*  percent: begフレームとendフレームの間のどこに居るのかを割合で指定します(0.0f-1.0f)。
	*  bone_top: ボーンの頂点を指定します。これが回転の中心座標となります。
	*  quate: 回転に使用するクォータニオンをここに指定します。
	* @return 現在のフレームの変換行列を返す。
	*/
	Matrix BoneMotion::trans_mat_of_bone(const MoveData & move_data, const float percent, const Coor3 & bone_top, const D3DXQUATERNION & quate) const
	{
		const Coor3 & beg_pos = move_data.beg->pos;
		const Coor3 & end_pos = move_data.end->pos;
		Coor3 now_pos = beg_pos + (end_pos - beg_pos)*percent;

		D3DXVECTOR3 scaling;
		scaling.x = 1.0f;
		scaling.y = 1.0f;
		scaling.z = 1.0f;

		D3DXVECTOR3 bone_top_dx;
		bone_top_dx.x = bone_top.x;
		bone_top_dx.y = bone_top.y;
		bone_top_dx.z = bone_top.z;

		D3DXVECTOR3 translation;
		translation.x = now_pos.x;
		translation.y = now_pos.y;
		translation.z = now_pos.z;

		D3DXMATRIX trans_mat;
		D3DXMatrixTransformation(
			&trans_mat,
			NULL,
			NULL,
			&scaling,
			&bone_top_dx,
			&quate,
			&translation);

		return Matrix(trans_mat);
	}

	bool BoneMotion::axis_is_unit(const Coor3 & axis) const
	{
		if (abs(axis.x) >= 0.00001f)
		{
			return false;
		}

		if (abs(axis.y) >= 0.00001f)
		{
			return false;
		}

		if (abs(axis.z - 1.0f) >= 0.00001f)
		{
			return false;
		}

		return true;
	}

}