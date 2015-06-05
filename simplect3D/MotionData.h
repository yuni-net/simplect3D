#ifndef MotionData_h_
#define MotionData_h_

#include "Top_pmd.h"
#include "popular.h"
#include "matrix.h"
#include "si3_coor3.h"
#include "BoneMap.h"
#include "BoneData.h"

namespace si3
{
	class manager;
	class ModelData;

	class MotionData
	{
	public:
		/***
		** ���[�V�����f�[�^�̃��[�h���s���܂��B
		** ���̎��_�ŃA�j���[�V�����ʒu���擪�ɐݒ肳��܂��B
		** �܂��A�A�j���[�V�������Đ���Ԃɂ��܂��B
		*/
		void load(manager & si3m, const char * path, const BoneMap & bone_map);

		/***
		** �A�j���[�V�������Đ���Ԃɂ��܂�
		*/
		void play();

		/***
		** �A�j���[�V�������ꎞ��~��Ԃɂ��܂�
		*/
		void pause();

		/***
		** �A�j���[�V�����ʒu��擪�ɐݒ肵�܂�
		*/
		void seek_first();

		/***
		** �A�j���[�V�������I�����Ă����true���A
		** �I�����Ă��Ȃ����false��Ԃ��܂��B
		*/
		bool did_finish_anime() const;




		/***
		** ���̊֐��̓��[�U�[���Ăяo���̂ł͂Ȃ��AMotion3D�N���X�����Ăяo�����s���֐��ł��B
		** �v���C��Ԃ̂Ƃ��ɃA�j���[�V�������s���܂��B
		** �A�j���[�V�����ɂ���ĕύX���ꂽ�V�������_����model_data�ɓ]�����܂��B
		*/
		void animation(ModelData & model_data);

		/***
		** �{�[���ƒ��_�̊֌W�f�[�^�𔒎��ɖ߂��܂��B
		*/
		void init_top_lists(const int bone_num);

		/***
		** �{�[���ƒ��_�̊֌W�f�[�^�ɐV�����֌W��ǉ����܂��B
		*/
		void add_top(int index, Top_pmd & top);

	private:


		bool animating;
		int now_frame;
		int final_frame;
		bool did_seek_first;
		fw::vector<BoneData> bone_lists;

		int bone_num() const;
	};
}

#endif