#include "Manager.h"


namespace si3
{

	bool Manager::init()
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
	bool Manager::begin_frame()
	{
		dxinputi.update();
		// todo
		return fw::Window::processage();	// プロセスメッセージを処理する
	}

	void Manager::register_display_object(const display_object & display_objecti)
	{
		displaym.register_display_object(display_objecti);
	}

	const ::si3::key & Manager::key() const
	{
		return dxinputi.key();
	}
	const ::si3::mouse & Manager::mouse() const
	{
		return dxinputi.mouse();
	}
	sound_manager & Manager::get_sound_manager()
	{
		return soundm;
	}

	::si3::camera & Manager::camera()
	{
		return dxcamerai;
	}
	const ::si3::camera & Manager::camera() const
	{
		return dxcamerai;
	}

	::si3::DisplayManager & Manager::DisplayManager()
	{
		return displaym;
	}

	LPDIRECT3DDEVICE9 Manager::get_dxdevice()
	{
		return dxm.get_dxdevice();
	}
	const LPDIRECT3DDEVICE9 Manager::get_dxdevice() const
	{
		return dxm.get_dxdevice();
	}

	/*
	* 描画内容を画面に反映する
	*
	*/
	void Manager::show()
	{
		dxcamerai.update();
		displaym.draw();

		fps60i.stop_if_need();	// 描画のタイミングを60fpsにする
	}


}