#include <MotionData.h>
#include <ModelData.h>
#include <cstdint>
#include <BoneMap.h>
#include <util_MMD.h>

namespace si3
{
	/***
	** モーションデータのロードを行います。
	** この時点でアニメーション位置が先頭に設定されます。
	*/
	void MotionData::load(manager & si3m, const char * path, const BoneMap & bone_map, const int bone_num)
	{
#pragma pack(push, 1)
		struct Header
		{
			char signature[30];
			char model_name[20];
		};

#pragma pack(pop)

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
		motion_list.setsize(bone_num);

		for (fw::uint motion_No = 0; motion_No < motion_num; ++motion_No)
		{
			Motion motion;
			file >> motion;

			const std::string & bone_name = get_text(motion.bone_name, 15);

			const int bone_index = bone_map.get_bone_index(bone_name);
			auto & bone = motion_list[bone_index];
			bone.add_key_frame(motion);

			if (motion.frame_No > final_frame)
			{
				final_frame = motion.frame_No;
			}
		}

		for (fw::uint motion_No = 0; motion_No < motion_list.size(); ++motion_No)
		{
			auto & motion = motion_list[motion_No];
			motion.finish_add_key_frame();
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
		did_seek_first = true;
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
	** この関数はユーザーが呼び出すのではなく、ModelDataクラス側が呼び出しを行う関数です。
	** プレイ状態のときにアニメーションカウンタを進めます。
	** アニメーションを行う必要が無い場合はtrueを返します。
	*/
	bool MotionData::animation()
	{
		if (animating == false && did_seek_first == false)
		{
			return true;
		}

		if (did_finish_anime())
		{
			return true;
		}

		++now_frame;

		did_seek_first = false;

		return false;
	}


	/***
	* @brief 現在のフレームのこのボーンの座標変換行列を計算します。
	* @param
	*  [out]trans_mat: 変換行列がここに格納されます
	*  [out]rot_mat: 変換行列の回転成分のみがここに格納されます
	*  bone_No: ボーン番号を指定します
	* @return
	*  座標を変換する必要がある場合はtrueを、そもそも変換する必要が無い場合はfalseを返します。
	*  例えば前のフレームから変化が無ければ座標を再度変換する必要は無いわけです。
	*  ただし、必ず１フレームずつ順番にアニメーションすることが前提になっています。
	*  逆再生や、任意のフレームにテレポートした場合は当然変換する必要があるでしょう。
	*/
	bool MotionData::compute_trans_mat(
		matrix & trans_mat,
		matrix & rot_mat,
		const coor3 & bone_pos,
		const int bone_No)
	{
		BoneMotion & motion = motion_list[bone_No];
		return motion.compute_trans_mat(trans_mat, rot_mat, bone_pos, now_frame);
	}








}

