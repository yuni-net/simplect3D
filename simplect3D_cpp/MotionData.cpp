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
		did_seek_first = true;
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
	** ���̊֐��̓��[�U�[���Ăяo���̂ł͂Ȃ��AModelData�N���X�����Ăяo�����s���֐��ł��B
	** �v���C��Ԃ̂Ƃ��ɃA�j���[�V�����J�E���^��i�߂܂��B
	** �A�j���[�V�������s���K�v�������ꍇ��true��Ԃ��܂��B
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
	* @brief ���݂̃t���[���̂��̃{�[���̍��W�ϊ��s����v�Z���܂��B
	* @param
	*  [out]trans_mat: �ϊ��s�񂪂����Ɋi�[����܂�
	*  [out]rot_mat: �ϊ��s��̉�]�����݂̂������Ɋi�[����܂�
	*  bone_No: �{�[���ԍ����w�肵�܂�
	* @return
	*  ���W��ϊ�����K�v������ꍇ��true���A���������ϊ�����K�v�������ꍇ��false��Ԃ��܂��B
	*  �Ⴆ�ΑO�̃t���[������ω���������΍��W���ēx�ϊ�����K�v�͖����킯�ł��B
	*  �������A�K���P�t���[�������ԂɃA�j���[�V�������邱�Ƃ��O��ɂȂ��Ă��܂��B
	*  �t�Đ���A�C�ӂ̃t���[���Ƀe���|�[�g�����ꍇ�͓��R�ϊ�����K�v������ł��傤�B
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

