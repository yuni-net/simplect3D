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
	void MotionData::load(manager & si3m, const char * path, const BoneMap & bone_map)
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

		for (fw::uint motion_No = 0; motion_No < motion_num; ++motion_No)
		{
			Motion motion;
			file >> motion;

			const std::string & bone_name = get_text(motion.bone_name, 15);

			const int bone_index = bone_map.get_bone_index(bone_name);
			auto & bone = bone_lists[bone_index];
			bone.add_key_frame(motion);

			if (motion.frame_No > final_frame)
			{
				final_frame = motion.frame_No;
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
			auto & bone = bone_lists[bone_No];
			bone.compute_trans_mat(now_frame);
		}

		model_data.unlock_top_buffer();

		did_seek_first = false;
	}




	/***
	** ボーンと頂点の関係データを白紙に戻します。
	*/
	void MotionData::init_top_lists(const int bone_num)
	{
		bone_lists.setsize(bone_num);
		for (fw::uint bone_No = 0; bone_No < bone_lists.size(); ++bone_No)
		{
			bone_lists[bone_No].init_top_lists();
		}
	}

	/***
	** ボーンと頂点の関係データに新しい関係を追加します。
	*/
	void MotionData::add_top(int index, Top_pmd & top)
	{
		int bonedex;
		bool is_main;

		bonedex = top.bone_num[0];
		is_main = true;
		bone_lists[bonedex].add_associated_top(index, top, is_main);

		// todo
#if 0
		bonedex = top.bone_num[1];
		is_main = false;
		bone_lists[bonedex].add_associated_top(index, top, is_main);
#endif

	}

	int MotionData::bone_num() const
	{
		return bone_lists.size();
	}






}

