#include "manager.h"


namespace si3
{

	bool manager::init()
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
	bool manager::begin_frame()
	{
		dxinputi.update();
		// todo
		return fw::Window::processage();
	}

	void manager::register_display_object(const display_object & display_objecti)
	{
		displaym.register_display_object(display_objecti);
	}

	const ::si3::key & manager::key() const
	{
		return dxinputi.key();
	}
	const ::si3::mouse & manager::mouse() const
	{
		return dxinputi.mouse();
	}
	sound_manager & manager::get_sound_manager()
	{
		return soundm;
	}

	::si3::camera & manager::camera()
	{
		return dxcamerai;
	}
	const ::si3::camera & manager::camera() const
	{
		return dxcamerai;
	}

	::si3::display_manager & manager::display_manager()
	{
		return displaym;
	}

	LPDIRECT3DDEVICE9 manager::get_dxdevice()
	{
		return dxm.get_dxdevice();
	}
	const LPDIRECT3DDEVICE9 manager::get_dxdevice() const
	{
		return dxm.get_dxdevice();
	}

	/*
	* 描画内容を画面に反映する
	*
	*/
	void manager::show()
	{
		dxcamerai.update();
		displaym.draw();

		fps60i.stop_if_need();	// 描画のタイミングを60fpsにする
	}


}