#ifndef MotionData_h_
#define MotionData_h_

#include "Top_pmd.h"
#include "popular.h"
#include "matrix.h"
#include "si3_coor3.h"
#include "BoneMap.h"
#include "BoneMotion.h"

namespace si3
{
	class Manager;
	class ModelData;

	class MotionData
	{
	public:
		/***
		** ���[�V�����f�[�^�̃��[�h���s���܂��B
		** ���̎��_�ŃA�j���[�V�����ʒu���擪�ɐݒ肳��܂��B
		** �܂��A�A�j���[�V�������Đ���Ԃɂ��܂��B
		*/
		void load(Manager & si3m, const char * path, const BoneMap & bone_map, const int bone_num);

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
		** ���̊֐��̓��[�U�[���Ăяo���̂ł͂Ȃ��AModelData�N���X�����Ăяo�����s���֐��ł��B
		** �v���C��Ԃ̂Ƃ��ɃA�j���[�V�����J�E���^��i�߂܂��B
		** �A�j���[�V�������s���K�v�������ꍇ��true��Ԃ��܂��B
		*/
		bool animation();

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
		bool compute_trans_mat(
			matrix & trans_mat,
			matrix & rot_mat,
			const coor3 & bone_pos,
			const int bone_No);



	private:


		bool animating;
		int now_frame;
		int final_frame;
		bool did_seek_first;
		fw::Array<BoneMotion> motion_list;
	};
}

#endif