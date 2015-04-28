#pragma once

#include "popular.h"
#include "display_object.h"
#include "dxmanager.h"
#include "display_manager.h"
#include "fps60.h"
#include "dxlight.h"
#include "camera.h"
#include "dxinput.h"
#include "key.h"
#include "sound_manager.h"

namespace si3
{
	class manager
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

		void register_display_object(const display_object & display_objecti);

		const ::si3::key & key() const;
		const ::si3::mouse & mouse() const;
		sound_manager & get_sound_manager();

		::si3::camera & camera();
		const ::si3::camera & camera() const;

		::si3::display_manager & display_manager();


		/*
		 * �`����e����ʂɔ��f����
		 *
		 */
		void show();




		LPDIRECT3DDEVICE9 get_dxdevice();
		const LPDIRECT3DDEVICE9 get_dxdevice() const;

	private:
		fw::window windowi;
		dxmanager dxm;
		::si3::display_manager displaym;
		fps60 fps60i;
		dxlight dxlighti;
		::si3::camera dxcamerai;
		dxinput dxinputi;
		sound_manager soundm;
	};
}