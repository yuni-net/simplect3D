#ifndef si3_BoneMotion_h_
#define si3_BoneMotion_h_

#include <list>
#include "popular.h"
#include "si3_coor3.h"
#include "Top_pmd.h"
#include "util_MMD.h"
#include "matrix.h"
#include "utility.h"

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
		* �S�ẴL�[�t���[�����̒ǉ����I��������K���Ăяo���Ă��������B
		*/
		void finish_add_key_frame();


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
			const Coor3 & bone_pos,
			const int now_frame);



	private:
		struct KeyFrame
		{
			int frame;
			Coor3 pos;
			Quaternion quate;

			bool operator<(const KeyFrame & another) const
			{
				return frame < another.frame;
			}
		};

		struct MoveData
		{
			const KeyFrame * beg;
			const KeyFrame * end;
		};


		
		std::vector<KeyFrame> key_frame_list;


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

		float compute_percent(MoveData & move_data, const int now_frame) const;

		/***
		* @brief move_data�Ɋ�Â����݂̃t���[���̉�]�s����v�Z���Ă����Ԃ��B
		* @param
		*  move_data: MoveData�̃C���X�^���X���w�肵�܂��B
		*  percent: beg�t���[����end�t���[���̊Ԃ̂ǂ��ɋ���̂��������Ŏw�肵�܂�(0.0f-1.0f)�B
		*  [out]quate: �v�Z�̌��ʓ���ꂽ�N�H�[�^�j�I���������Ɋi�[����܂��B
		* @return ���݂̃t���[���̉�]�s���Ԃ��B
		*/
		matrix rot_mat_of_bone(
			const MoveData & move_data,
			const float percent,
			D3DXQUATERNION & quate) const;

	//	matrix para_mat_of_bone(const MoveData & move_data, const float percent) const;

		/***
		* @brief move_data�Ɋ�Â����݂̃t���[���̕ϊ��s����v�Z���Ă����Ԃ��B
		* @param
		*  move_data: MoveData�̃C���X�^���X���w�肵�܂��B
		*  percent: beg�t���[����end�t���[���̊Ԃ̂ǂ��ɋ���̂��������Ŏw�肵�܂�(0.0f-1.0f)�B
		*  bone_top: �{�[���̒��_���w�肵�܂��B���ꂪ��]�̒��S���W�ƂȂ�܂��B
		*  quate: ��]�Ɏg�p����N�H�[�^�j�I���������Ɏw�肵�܂��B
		* @return ���݂̃t���[���̕ϊ��s���Ԃ��B
		*/
		matrix trans_mat_of_bone(
			const MoveData & move_data,
			const float percent,
			const Coor3 & bone_top,
			const D3DXQUATERNION & quate) const;


		bool axis_is_unit(const Coor3 & axis) const;
	};
}

#endif