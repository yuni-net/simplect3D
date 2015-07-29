#include "si3_Manager.h"


namespace si3
{
	/*
	* @return true:成功 false:失敗
	*
	*/
	bool Manager::init()
	{
		return get_instance().init_dynamic();
	}

	/*
	* 毎フレーム最初に呼び出すこと
	* @return true:続行可能 false:続行不可能
	*
	*/
	bool Manager::begin_frame()
	{
		return get_instance().begin_frame_dynamic();
	}

	/***
	@brief 表示リストにオブジェクトを登録する。
	@detail この登録はこのフレームでのみ有効であるため、
	毎フレーム描画したければ毎フレーム登録し直す必要がある。
	*/
	void Manager::register_display_object(const DisplayObject & display_objecti)
	{
		get_instance().register_display_object_dynamic(display_objecti);
	}

	/***
	@brief キーボード情報を管理するクラスのインスタンスを得る。
	*/
	const ::si3::Key & Manager::key()
	{
		return get_instance().key_dynamic();
	}

	/***
	@brief マウス情報を管理するクラスのインスタンスを得る。
	*/
	const ::si3::Mouse & Manager::mouse()
	{
		return get_instance().mouse_dynamic();
	}

	/***
	@brief サウンド情報を管理するクラスのインスタンスを得る。
	*/
	SoundManager & Manager::get_sound_manager()
	{
		return get_instance().get_sound_manager_dynamic();
	}

	/***
	@brief カメラを管理するクラスのインスタンスを得る。
	*/
	::si3::Camera & Manager::camera()
	{
		return get_instance().camera_dynamic();
	}

	/***
	@brief ディスプレイを管理するクラスのインスタンスを得る。
	*/
	::si3::DisplayManager & Manager::display_manager()
	{
		return get_instance().display_manager_dynamic();
	}


	/*
	* 描画内容を画面に反映する
	*
	*/
	void Manager::show()
	{
		get_instance().show_dynamic();
	}




	LPDIRECT3DDEVICE9 Manager::get_dxdevice()
	{
		return get_instance().get_dxdevice_dynamic();
	}
	Manager & Manager::get_instance()
	{
		static Manager manager;
		return manager;
	}




	bool Manager::init_dynamic()
	{
		fps60i.init();

		windowi.width(640).height(480);
		windowi.create();

		if (dxm.init(windowi.myhandle(), windowi.width(), windowi.height())==false) return false;

		if (soundm.init() == false) return false;
		displaym.init(dxm.get_present_parameters(), dxm.get_dxdevice());
		if(dxlighti.init(dxm.get_dxdevice())==false) return false;
		if(dxcamerai.init(dxm.get_dxdevice(), windowi.width(), windowi.height())==false) return false;
		if(dxinputi.init(windowi.myhandle())==false) return false;

		return true;
	}

	/*
	* 毎フレーム最初に呼び出すこと
	* 続行可能ならtrue、不可能ならfalseを返す
	*
	*/
	bool Manager::begin_frame_dynamic()
	{
		dxinputi.update();
		// todo
		return fw::Window::processage();	// プロセスメッセージを処理する
	}

	void Manager::register_display_object_dynamic(const DisplayObject & display_objecti)
	{
		displaym.register_display_object(display_objecti);
	}

	const ::si3::Key & Manager::key_dynamic() const
	{
		return dxinputi.key();
	}
	const ::si3::Mouse & Manager::mouse_dynamic() const
	{
		return dxinputi.mouse();
	}
	SoundManager & Manager::get_sound_manager_dynamic()
	{
		return soundm;
	}

	::si3::Camera & Manager::camera_dynamic()
	{
		return dxcamerai;
	}

	::si3::DisplayManager & Manager::display_manager_dynamic()
	{
		return displaym;
	}

	LPDIRECT3DDEVICE9 Manager::get_dxdevice_dynamic()
	{
		return dxm.get_dxdevice();
	}

	/*
	* 描画内容を画面に反映する
	*
	*/
	void Manager::show_dynamic()
	{
		dxcamerai.update();
		displaym.draw();

		fps60i.stop_if_need();	// 描画のタイミングを60fpsにする
	}


}