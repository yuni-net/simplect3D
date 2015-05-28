#include <MotionData.h>
#include <ModelData.h>

namespace si3
{
	/***
	** ���[�V�����f�[�^�̃��[�h���s���܂��B
	** ���̎��_�ŃA�j���[�V�����ʒu���擪�ɐݒ肳��܂��B
	*/
	void MotionData::load(manager & si3m, const char * path)
	{
		// todo
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
	** ���̊֐��̓��[�U�[���Ăяo���̂ł͂Ȃ��AMotion3D�N���X�����Ăяo�����s���֐��ł��B
	** �v���C��Ԃ̂Ƃ��ɃA�j���[�V�������s���܂��B
	** �A�j���[�V�����ɂ���ĕύX���ꂽ�V�������_����model_data�ɓ]�����܂��B
	*/
	void MotionData::animation(ModelData & model_data)
	{
		// todo
	}

	/***
	** �{�[���ƒ��_�̊֌W�f�[�^�𔒎��ɖ߂��܂��B
	*/
	void MotionData::clear_top_lists()
	{
		top_lists.zerosize();
	}

	/***
	** �{�[���ƒ��_�̊֌W�f�[�^�ɐV�����֌W��ǉ����܂��B
	*/
	void MotionData::add_top(Top_pmd & top)
	{
		int bonedex;
		BonedTop boned_top;

		bonedex = top.bone_num[0];
		boned_top.init(top, top.bone_weight);
		top_lists[bonedex].add(boned_top);

		// todo add bone2
	}

}