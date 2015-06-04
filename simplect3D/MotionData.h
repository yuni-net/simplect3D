#ifndef MotionData_h_
#define MotionData_h_

#include "Top_pmd.h"
#include "popular.h"
#include "matrix.h"
#include "si3_coor3.h"
#include "BoneMap.h"

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
		struct BonedTop
		{
			int index;
			float pos[3];
			float normal[3];
			float bone_weight;

			void init(int index, const Top_pmd & top, float bone_weight)
			{
				this->index = index;

				pos[0] = top.pos[0];
				pos[1] = top.pos[1];
				pos[2] = top.pos[2];

				normal[0] = top.normal[0];
				normal[1] = top.normal[1];
				normal[2] = top.normal[2];

				bone_weight = top.bone_weight;
			}
		};

		struct KeyFrame
		{
			int frame;
			coor3 pos;
			float radian;
			coor3 axis;
		};

		struct MoveData
		{
			const KeyFrame * beg;
			const KeyFrame * end;
		};

		struct BoneData
		{
			fw::vector<BonedTop> associated_tops;
			fw::vector<KeyFrame> key_frame_list;
		};

		bool animating;
		int now_frame;
		int final_frame;
		bool did_seek_first;
		fw::vector<BoneData> bone_lists;

		int bone_num() const;
		bool did_bone_move(MoveData & move_data, int bone_No) const;
		matrix rot_mat_of_bone(const MoveData & move_data) const;
		matrix para_mat_of_bone(const MoveData & move_data) const;
		int top_num_of_bone(int bone_No) const;
		const BonedTop & top_of_bone(int bone_No, int top_No);
		void trans_and_set(D3DVECTOR & target, const float base[3], const matrix & mat);
	};
}

#endif