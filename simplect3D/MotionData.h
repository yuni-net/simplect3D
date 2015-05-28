#ifndef MotionData_h_
#define MotionData_h_

#include "Top_pmd.h"
#include "popular.h"

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
		void load(manager & si3m, const char * path);

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
		** ���̊֐��̓��[�U�[���Ăяo���̂ł͂Ȃ��AMotion3D�N���X�����Ăяo�����s���֐��ł��B
		** �v���C��Ԃ̂Ƃ��ɃA�j���[�V�������s���܂��B
		** �A�j���[�V�����ɂ���ĕύX���ꂽ�V�������_����model_data�ɓ]�����܂��B
		*/
		void animation(ModelData & model_data);

		/***
		** �{�[���ƒ��_�̊֌W�f�[�^�𔒎��ɖ߂��܂��B
		*/
		void clear_top_lists();

		/***
		** �{�[���ƒ��_�̊֌W�f�[�^�ɐV�����֌W��ǉ����܂��B
		*/
		void add_top(Top_pmd & top);

	private:
		struct BonedTop
		{
			float pos[3];
			float normal[3];
			unsigned char bone_weight;

			void init(const Top_pmd & top, unsigned char bone_weight)
			{
				pos[0] = top.pos[0];
				pos[1] = top.pos[1];
				pos[2] = top.pos[2];

				normal[0] = top.normal[0];
				normal[1] = top.normal[1];
				normal[2] = top.normal[2];

				bone_weight = top.bone_weight;
			}
		};

		bool animating;
		int now_frame;
		bool did_seek_first;
		fw::vector<fw::vector<BonedTop> > top_lists;
	};
}

#endif