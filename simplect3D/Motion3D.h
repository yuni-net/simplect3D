#ifndef Motion3D_h_
#define Motion3D_h_

#include "si3_coor3.h"
#include "display_object.h"
#include "Matrix.h"
#include "MotionData.h"
#include "ModelData_pmd.h"

namespace si3
{
	class Manager;

	class Motion3D :public DisplayObject
	{
	public:
		Coor3 pos;
		Coor3 radian;

		/***
		** ���f���f�[�^�ƃ��[�V�����f�[�^��ǂݍ��݂܂��B
		** ���f���f�[�^��pmd�`���A���[�V�����f�[�^��vmd�`���̂ݑΉ��ł��B
		** ���̎��_�ŃA�j���[�V�����ʒu���擪�ɐݒ肳��܂��B
		** �܂��A��]�p��xyz�S�ă[���ɐݒ肳��܂��B
		*/
		void load(Manager & si3m, const char * model_path, const char * motion_path);

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






		// @override
		/***
		** ���̊֐��̓��[�U�[�����Ăяo���̂ł͂Ȃ��Asimplect3D���C�u�����̃V�X�e�����Ăяo���܂��B
		** ���̎��_�ŃA�j���[�V�������s���܂��B
		*/
		void draw_no_alpha() const;

		// @override
		/***
		** ���̊֐��̓��[�U�[�����Ăяo���̂ł͂Ȃ��Asimplect3D���C�u�����̃V�X�e�����Ăяo���܂��B
		*/
		void draw_alpha() const;



	private:
		mutable ModelData_pmd model_data;
		mutable MotionData motion_data;
		BoneMap bone_map;
		mutable Matrix world_mat;
		LPDIRECT3DDEVICE9 device;

		void compute_world_mat() const;
	};
}

#endif