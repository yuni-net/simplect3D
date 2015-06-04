#include <MotionData.h>
#include <ModelData.h>
#include <cstdint>
#include <BoneMap.h>

namespace si3
{
	/***
	** モーションデータのロードを行います。
	** この時点でアニメーション位置が先頭に設定されます。
	*/
	void MotionData::load(manager & si3m, const char * path, const BoneMap & bone_map)
	{
#pragma pack(1)
		struct Header
		{
			char signature[30];
			char model_name[20];
		};

		struct Motion
		{
			char bone_name[15];
			int32_t frame_No;
			float location[3];
			float rotation[4];
			int8_t interpolation[64];
		};
#pragma pack()

		final_frame = 0;

		fw::bfile file(path);
		file.open_to_read();

		Header header;
		file >> header;
		const std::string signature("Vocaloid Motion Data 0002");
		header.signature[signature.length()] = '\0';
		if (header.signature != signature)
		{
			// This is not vmd file.
			return;
		}

		unsigned long motion_num;
		file >> motion_num;

		for (fw::uint motion_No = 0; motion_No < motion_num; ++motion_No)
		{
			Motion motion;
			file >> motion;

			char bone_name[16];
			memcpy(bone_name, motion.bone_name, 15);
			bone_name[15] = '\0';
			int bone_index = bone_map.get_bone_index(bone_name);
			auto & bone = bone_lists[bone_index];

			KeyFrame key_frame;
			key_frame.frame = motion.frame_No;
			key_frame.pos.x = motion.location[0];
			key_frame.pos.y = motion.location[1];
			key_frame.pos.z = motion.location[2];
			key_frame.radian = motion.rotation[0];
			key_frame.axis.x = motion.rotation[1];
			key_frame.axis.y = motion.rotation[2];
			key_frame.axis.z = motion.rotation[3];

			bone.key_frame_list.add(key_frame);

			if (key_frame.frame > final_frame)
			{
				final_frame = key_frame.frame;
			}
		}

		animating = true;
		now_frame = 0;
		did_seek_first = false;
	}

	/***
	** アニメーションを再生状態にします
	*/
	void MotionData::play()
	{
		animating = true;
	}

	/***
	** アニメーションを一時停止状態にします
	*/
	void MotionData::pause()
	{
		animating = false;
	}

	/***
	** アニメーション位置を先頭に設定します
	*/
	void MotionData::seek_first()
	{
		now_frame = 0;
	}

	/***
	** アニメーションが終了していればtrueを、
	** 終了していなければfalseを返します。
	*/
	bool MotionData::did_finish_anime() const
	{
		return final_frame < now_frame;
	}





	/***
	** この関数はユーザーが呼び出すのではなく、Motion3Dクラス側が呼び出しを行う関数です。
	** プレイ状態のときにアニメーションを行います。
	** アニメーションによって変更された新しい頂点情報をmodel_dataに転送します。
	*/
	void MotionData::animation(ModelData & model_data)
	{
		if (animating == false && did_seek_first == false)
		{
			return;
		}

		if (did_finish_anime())
		{
			return;
		}

		++now_frame;

		top_type * top_buffer = model_data.lock_top_buffer();

		for (int bone_No = 0; bone_No < bone_num(); ++bone_No)
		{
			MoveData move_data;
			if (did_bone_move(move_data, bone_No) == false)
			{
				continue;
			}

			matrix rot_mat = rot_mat_of_bone(move_data);
			matrix para_mat = para_mat_of_bone(move_data);
			matrix trans_mat = rot_mat * para_mat;

			for (int top_No = 0; top_No < top_num_of_bone(bone_No); ++top_No)
			{
				const BonedTop & base_top = top_of_bone(bone_No, top_No);
				top_type & target_top = top_buffer[base_top.index];

				trans_and_set(target_top.pos, base_top.pos, trans_mat);		// target_top.pos = base_top.pos * trans
				trans_and_set(target_top.normal, base_top.normal, rot_mat);	// target_top.normal = base_top.normal * rot
			}
		}

		model_data.unlock_top_buffer();

		did_seek_first = false;
	}


	void MotionData::trans_and_set(D3DVECTOR & target, const float base[3], const matrix & mat)
	{
		matrix coor_mat;
		coor_mat.x(base[0]);
		coor_mat.y(base[1]);
		coor_mat.z(base[2]);
		coor_mat *= mat;
		target.x = coor_mat.x();
		target.y = coor_mat.y();
		target.z = coor_mat.z();
	}


	/***
	** ボーンと頂点の関係データを白紙に戻します。
	*/
	void MotionData::init_top_lists(const int bone_num)
	{
		bone_lists.setsize(bone_num);
		for (fw::uint bone_No = 0; bone_No < bone_lists.size(); ++bone_No)
		{
			bone_lists[bone_No].associated_tops.zerosize();
		}
	}

	/***
	** ボーンと頂点の関係データに新しい関係を追加します。
	*/
	void MotionData::add_top(int index, Top_pmd & top)
	{
		int bonedex;
		BonedTop boned_top;

		bonedex = top.bone_num[0];
		boned_top.init(index, top, top.bone_weight/100.0f);
		bone_lists[bonedex].associated_tops.add(boned_top);

		bonedex = top.bone_num[1];
		if (bonedex == 0)
		{
			return;
		}

		boned_top.init(index, top, (100-top.bone_weight)/100.0f);
		bone_lists[bonedex].associated_tops.add(boned_top);
	}

	int MotionData::bone_num() const
	{
		return bone_lists.size();
	}

	bool MotionData::did_bone_move(MoveData & move_data, int bone_No) const
	{
		const fw::vector<KeyFrame> & key_frame_list = bone_lists[bone_No].key_frame_list;

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
			if (now_frame >= key_frame)
			{
				// 現在の位置がわかったら脱出
				break;
			}

			// 現在位置は確定ではないが暫定的に進んだところにしておく
			beg_index = frame_No;
		}

		move_data.beg = &(key_frame_list[beg_index]);
		move_data.end = &(key_frame_list[beg_index+1]);

		return true;
	}

	matrix MotionData::rot_mat_of_bone(const MoveData & move_data) const
	{
		const float beg_rot = move_data.beg->radian;
		const float end_rot = move_data.end->radian;
		const float ave_rot = (beg_rot + end_rot)*0.5f;

		const auto & axis = move_data.beg->axis;
		D3DXQUATERNION quate(axis.x, axis.y, axis.z, ave_rot);

		D3DXMATRIX rot_mat;
		D3DXMatrixRotationQuaternion(&rot_mat, &quate);

		return matrix(rot_mat);
	}
	matrix MotionData::para_mat_of_bone(const MoveData & move_data) const
	{
		const coor3 & beg_pos = move_data.beg->pos;
		const coor3 & end_pos = move_data.end->pos;
		coor3 ave_pos = (beg_pos + end_pos)*0.5f;
		return matrix().parallel(ave_pos.x, ave_pos.y, ave_pos.z);
	}

	int MotionData::top_num_of_bone(int bone_No) const
	{
		return bone_lists[bone_No].associated_tops.size();
	}

	const MotionData::BonedTop & MotionData::top_of_bone(int bone_No, int top_No)
	{
		return bone_lists[bone_No].associated_tops[top_No];
	}


}

