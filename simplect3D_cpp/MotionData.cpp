#include <MotionData.h>
#include <ModelData.h>
#include <cstdint>
#include <BoneMap.h>
#include <util_MMD.h>

namespace si3
{
	/***
	** ���[�V�����f�[�^�̃��[�h���s���܂��B
	** ���̎��_�ŃA�j���[�V�����ʒu���擪�ɐݒ肳��܂��B
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
	** �A�j���[�V�������Đ���Ԃɂ��܂�
	*/
	void MotionData::play()
	{
		animating = true;
	}

	/***
	** �A�j���[�V�������ꎞ��~��Ԃɂ��܂�
	*/
	void MotionData::pause()
	{
		animating = false;
	}

	/***
	** �A�j���[�V�����ʒu��擪�ɐݒ肵�܂�
	*/
	void MotionData::seek_first()
	{
		now_frame = 0;
	}

	/***
	** �A�j���[�V�������I�����Ă����true���A
	** �I�����Ă��Ȃ����false��Ԃ��܂��B
	*/
	bool MotionData::did_finish_anime() const
	{
		return final_frame < now_frame;
	}





	/***
	** ���̊֐��̓��[�U�[���Ăяo���̂ł͂Ȃ��AMotion3D�N���X�����Ăяo�����s���֐��ł��B
	** �v���C��Ԃ̂Ƃ��ɃA�j���[�V�������s���܂��B
	** �A�j���[�V�����ɂ���ĕύX���ꂽ�V�������_����model_data�ɓ]�����܂��B
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
	** �{�[���ƒ��_�̊֌W�f�[�^�𔒎��ɖ߂��܂��B
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
	** �{�[���ƒ��_�̊֌W�f�[�^�ɐV�����֌W��ǉ����܂��B
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

