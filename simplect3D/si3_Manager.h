#pragma once

#include "si3_popular.h"
#include "si3_DisplayObject.h"
#include "si3_DxManager.h"
#include "si3_DisplayManager.h"
#include "si3_Fps60.h"
#include "si3_DxLight.h"
#include "si3_Camera.h"
#include "si3_DxInput.h"
#include "si3_Key.h"
#include "si3_SoundManager.h"

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
		static bool init();

		/*
		 * @param
		 *      width: �E�B���h�E�̉���(px)
		 *      height: �E�B���h�E�̏c��(px)
		 * @return true:���� false:���s
		 *
		 */
		static bool init(const int width, const int height, const std::string & title);

		/*
		* ���t���[���ŏ��ɌĂяo������
		* @return true:���s�\ false:���s�s�\
		*
		*/
		static bool begin_frame();

		/***
		@brief �\�����X�g�ɃI�u�W�F�N�g��o�^����B
		@detail ���̓o�^�͂��̃t���[���ł̂ݗL���ł��邽�߁A
		���t���[���`�悵������Ζ��t���[���o�^�������K�v������B
		*/
		static void register_display_object(const DisplayObject & display_objecti);

		/***
		@brief �L�[�{�[�h�����Ǘ�����N���X�̃C���X�^���X�𓾂�B
		*/
		static const ::si3::Key & key();

		/***
		@brief �}�E�X�����Ǘ�����N���X�̃C���X�^���X�𓾂�B
		*/
		static const ::si3::Mouse & mouse();

		/***
		@brief �T�E���h�����Ǘ�����N���X�̃C���X�^���X�𓾂�B
		*/
		static SoundManager & get_sound_manager();

		/***
		@brief �J�������Ǘ�����N���X�̃C���X�^���X�𓾂�B
		*/
		static ::si3::Camera & camera();

		/***
		@brief �f�B�X�v���C���Ǘ�����N���X�̃C���X�^���X�𓾂�B
		*/
		static ::si3::DisplayManager & display_manager();


		/*
		* �`����e����ʂɔ��f����
		*
		*/
		static void show();




		static LPDIRECT3DDEVICE9 get_dxdevice();
		static Manager & get_instance();

	private:
		fw::Window windowi;
		DxManager dxm;
		::si3::DisplayManager displaym;
		Fps60 fps60i;
		DxLight dxlighti;
		::si3::Camera dxcamerai;
		DxInput dxinputi;
		SoundManager soundm;
		int window_width;
		int window_height;

		/*
		 * @param
		 *      width: �E�B���h�E�̉���(px)
		 *      height: �E�B���h�E�̏c��(px)
		 *		title: �E�B���h�E�^�C�g��
		 * @return true:���� false:���s
		 *
		 */
		bool init_dynamic(const int width, const int height, const std::string & title);

		/*
		 * ���t���[���ŏ��ɌĂяo������
		 * @return true:���s�\ false:���s�s�\
		 *
		 */
		bool begin_frame_dynamic();

		/***
		@brief �\�����X�g�ɃI�u�W�F�N�g��o�^����B
		@detail ���̓o�^�͂��̃t���[���ł̂ݗL���ł��邽�߁A
		        ���t���[���`�悵������Ζ��t���[���o�^�������K�v������B
		*/
		void register_display_object_dynamic(const DisplayObject & display_objecti);

		/***
		@brief �L�[�{�[�h�����Ǘ�����N���X�̃C���X�^���X�𓾂�B
		*/
		const ::si3::Key & key_dynamic() const;

		/***
		@brief �}�E�X�����Ǘ�����N���X�̃C���X�^���X�𓾂�B
		*/
		const ::si3::Mouse & mouse_dynamic() const;

		/***
		@brief �T�E���h�����Ǘ�����N���X�̃C���X�^���X�𓾂�B
		*/
		SoundManager & get_sound_manager_dynamic();

		/***
		@brief �J�������Ǘ�����N���X�̃C���X�^���X�𓾂�B
		*/
		::si3::Camera & camera_dynamic();

		/***
		@brief �f�B�X�v���C���Ǘ�����N���X�̃C���X�^���X�𓾂�B
		*/
		::si3::DisplayManager & display_manager_dynamic();


		/*
		 * �`����e����ʂɔ��f����
		 *
		 */
		void show_dynamic();




		LPDIRECT3DDEVICE9 get_dxdevice_dynamic();
	};
}