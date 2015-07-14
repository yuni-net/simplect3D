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
	/***
	simplect3Dライブラリ全体を管理するクラス。
	*/
	class Manager
	{
	public:
		/*
		 * @return true:成功 false:失敗
		 *
		 */
		bool init();

		/*
		 * 毎フレーム最初に呼び出すこと
		 * @return true:続行可能 false:続行不可能
		 *
		 */
		bool begin_frame();

		/***
		@brief 表示リストにオブジェクトを登録する。
		@detail この登録はこのフレームでのみ有効であるため、
		        毎フレーム描画したければ毎フレーム登録し直す必要がある。
		*/
		void register_display_object(const display_object & display_objecti);

		/***
		@brief キーボード情報を管理するクラスのインスタンスを得る。
		*/
		const ::si3::key & key() const;

		/***
		@brief マウス情報を管理するクラスのインスタンスを得る。
		*/
		const ::si3::mouse & mouse() const;

		/***
		@brief サウンド情報を管理するクラスのインスタンスを得る。
		*/
		sound_manager & get_sound_manager();

		/***
		@brief カメラを管理するクラスのインスタンスを得る。
		*/
		::si3::camera & camera();
		const ::si3::camera & camera() const;

		/***
		@brief ディスプレイを管理するクラスのインスタンスを得る。
		*/
		::si3::display_manager & display_manager();


		/*
		 * 描画内容を画面に反映する
		 *
		 */
		void show();




		LPDIRECT3DDEVICE9 get_dxdevice();
		const LPDIRECT3DDEVICE9 get_dxdevice() const;

	private:
		fw::Window windowi;
		dxmanager dxm;
		::si3::display_manager displaym;
		fps60 fps60i;
		dxlight dxlighti;
		::si3::camera dxcamerai;
		dxinput dxinputi;
		sound_manager soundm;
	};
}