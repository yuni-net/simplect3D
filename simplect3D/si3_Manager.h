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
	simplect3Dライブラリ全体を管理するクラス。
	*/
	class Manager
	{
	public:
		/*
		* @return true:成功 false:失敗
		*
		*/
		static bool init();

		/*
		 * @param
		 *      width: ウィンドウの横幅(px)
		 *      height: ウィンドウの縦幅(px)
		 * @return true:成功 false:失敗
		 *
		 */
		static bool init(const int width, const int height, const std::string & title);

		/*
		* 毎フレーム最初に呼び出すこと
		* @return true:続行可能 false:続行不可能
		*
		*/
		static bool begin_frame();

		/***
		@brief 表示リストにオブジェクトを登録する。
		@detail この登録はこのフレームでのみ有効であるため、
		毎フレーム描画したければ毎フレーム登録し直す必要がある。
		*/
		static void register_display_object(const DisplayObject & display_objecti);

		/***
		@brief キーボード情報を管理するクラスのインスタンスを得る。
		*/
		static const ::si3::Key & key();

		/***
		@brief マウス情報を管理するクラスのインスタンスを得る。
		*/
		static const ::si3::Mouse & mouse();

		/***
		@brief サウンド情報を管理するクラスのインスタンスを得る。
		*/
		static SoundManager & get_sound_manager();

		/***
		@brief カメラを管理するクラスのインスタンスを得る。
		*/
		static ::si3::Camera & camera();

		/***
		@brief ディスプレイを管理するクラスのインスタンスを得る。
		*/
		static ::si3::DisplayManager & display_manager();


		/*
		* 描画内容を画面に反映する
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
		 *      width: ウィンドウの横幅(px)
		 *      height: ウィンドウの縦幅(px)
		 *		title: ウィンドウタイトル
		 * @return true:成功 false:失敗
		 *
		 */
		bool init_dynamic(const int width, const int height, const std::string & title);

		/*
		 * 毎フレーム最初に呼び出すこと
		 * @return true:続行可能 false:続行不可能
		 *
		 */
		bool begin_frame_dynamic();

		/***
		@brief 表示リストにオブジェクトを登録する。
		@detail この登録はこのフレームでのみ有効であるため、
		        毎フレーム描画したければ毎フレーム登録し直す必要がある。
		*/
		void register_display_object_dynamic(const DisplayObject & display_objecti);

		/***
		@brief キーボード情報を管理するクラスのインスタンスを得る。
		*/
		const ::si3::Key & key_dynamic() const;

		/***
		@brief マウス情報を管理するクラスのインスタンスを得る。
		*/
		const ::si3::Mouse & mouse_dynamic() const;

		/***
		@brief サウンド情報を管理するクラスのインスタンスを得る。
		*/
		SoundManager & get_sound_manager_dynamic();

		/***
		@brief カメラを管理するクラスのインスタンスを得る。
		*/
		::si3::Camera & camera_dynamic();

		/***
		@brief ディスプレイを管理するクラスのインスタンスを得る。
		*/
		::si3::DisplayManager & display_manager_dynamic();


		/*
		 * 描画内容を画面に反映する
		 *
		 */
		void show_dynamic();




		LPDIRECT3DDEVICE9 get_dxdevice_dynamic();
	};
}