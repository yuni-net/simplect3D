#ifndef si3_BoneMotion_h_
#define si3_BoneMotion_h_

#include "popular.h"
#include "si3_coor3.h"
#include "Top_pmd.h"
#include "util_MMD.h"
#include "matrix.h"

namespace si3
{
	class BoneMotion
	{
	public:

		/***
		* @brief �L�[�t���[������ǉ����܂��B
		*/
		void add_key_frame(const Motion & motion);


		/***
		* @brief ���݂̃t���[���̂��̃{�[���̍��W�ϊ��s����v�Z���܂��B
		* @param
		*  [out]trans_mat: �ϊ��s�񂪂����Ɋi�[����܂�
		*  [out]rot_mat: �ϊ��s��̉�]�����݂̂������Ɋi�[����܂�
		*  now_frame: ���݂̃t���[���w�肵�܂�
		* @return
		*  ���W��ϊ�����K�v������ꍇ��true���A���������ϊ�����K�v�������ꍇ��false��Ԃ��܂��B
		*  �Ⴆ�ΑO�̃t���[������ω���������΍��W���ēx�ϊ�����K�v�͖����킯�ł��B
		*  �������A�K���P�t���[�������ԂɃA�j���[�V�������邱�Ƃ��O��ɂȂ��Ă��܂��B
		*  �t�Đ���A�C�ӂ̃t���[���Ƀe���|�[�g�����ꍇ�͓��R�ϊ�����K�v������ł��傤�B
		*/
		bool compute_trans_mat(
			matrix & trans_mat,
			matrix & rot_mat,
			const int now_frame);



	private:
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


		
		fw::vector<KeyFrame> key_frame_list;


		void compute_move_data(
			MoveData & move_data,
			bool & todo_trans,
			bool & todo_judge_move,
			const int now_frame);

		/***
		* move_data��beg��end���قȂ�l�ł��邩�ǂ����𔻒肵�܂��B
		* �قȂ�ꍇ��true���A�����ł���ꍇ��false��Ԃ��܂��B
		*/
		bool did_bone_move(MoveData & move_data) const;

		matrix rot_mat_of_bone(const MoveData & move_data) const;
		matrix para_mat_of_bone(const MoveData & move_data) const;

	};
}

#endif