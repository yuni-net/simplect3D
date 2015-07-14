#pragma once

#include "popular.h"
#include "display_object.h"
#include "DxManager.h"
#include "display_manager.h"
#include "fps60.h"
#include "dxlight.h"
#include "camera.h"
#include "dxinput.h"
#include "key.h"
#include "sound_manager.h"

namespace si3
{
	/***
	simplect3D���C�u�����S�̂��Ǘ�����N���X�B
	*/
	class Manager
	{
	public:
		/*
		 * @return true:���� false:���s
		 *
		 */
		bool init();

		/*
		 * ���t���[���ŏ��ɌĂяo������
		 * @return true:���s�\ false:���s�s�\
		 *
		 */
		bool begin_frame();

		/***
		@brief �\�����X�g�ɃI�u�W�F�N�g��o�^����B
		@detail ���̓o�^�͂��̃t���[���ł̂ݗL���ł��邽�߁A
		        ���t���[���`�悵������Ζ��t���[���o�^�������K�v������B
		*/
		void register_display_object(const DisplayObject & display_objecti);

		/***
		@brief �L�[�{�[�h�����Ǘ�����N���X�̃C���X�^���X�𓾂�B
		*/
		const ::si3::Key & key() const;

		/***
		@brief �}�E�X�����Ǘ�����N���X�̃C���X�^���X�𓾂�B
		*/
		const ::si3::Mouse & mouse() const;

		/***
		@brief �T�E���h�����Ǘ�����N���X�̃C���X�^���X�𓾂�B
		*/
		SoundManager & get_sound_manager();

		/***
		@brief �J�������Ǘ�����N���X�̃C���X�^���X�𓾂�B
		*/
		::si3::Camera & camera();
		const ::si3::Camera & camera() const;

		/***
		@brief �f�B�X�v���C���Ǘ�����N���X�̃C���X�^���X�𓾂�B
		*/
		::si3::DisplayManager & DisplayManager();


		/*
		 * �`����e����ʂɔ��f����
		 *
		 */
		void show();




		LPDIRECT3DDEVICE9 get_dxdevice();
		const LPDIRECT3DDEVICE9 get_dxdevice() const;

	private:
		fw::Window windowi;
		DxManager dxm;
		::si3::DisplayManager displaym;
		Fps60 fps60i;
		DxLight dxlighti;
		::si3::Camera dxcamerai;
		DxInput dxinputi;
		SoundManager soundm;
	};
}