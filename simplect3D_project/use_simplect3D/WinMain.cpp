/*
** GM31 No.01
** v2.1.0
*/

#include <simplect3D.h>



int WINAPI WinMain(HINSTANCE hI, HINSTANCE hP, LPSTR lpC, int nC)
{
	fw::xrandom_init(static_cast<unsigned long>(time(NULL)));

	si3::Manager * si3mp = new si3::Manager();
	si3::Manager & si3m = *si3mp;
	if (si3m.init() == false)
	{
		fw::popup("failed");
		return -1;
	}

	const si3::Key & key = si3m.key();
	si3::Camera & camera = si3m.camera();
	const si3::Mouse & mouse = si3m.mouse();
	auto & displaym = si3m.display_manager();

	displaym.context_color(1.0f, 1.0f, 1.0f);

	auto & skydome = si3m.display_manager().get_skydome();
	skydome.load("data/orange.jpg");

	camera.x(0.0f);
	camera.y(10.0f);
	camera.z(-32.0f);

	camera.look_x(0.0f);
	camera.look_y(10.0f);
	camera.look_z(0.0f);

	si3::Motion3D yuuki;
	yuuki.load("data/yuuki/yuuki_before.pmd", "data/motion05.vmd");
//	yuuki.pause();
	yuuki.pos.x = 0.0f;
	yuuki.pos.y = 0.0f;
	yuuki.pos.z = 0.0f;

	float fRadian = -3.14159265f/2.0f;
	const float fRotSpeed = 0.08f;
	float fRadius = 14.0f;

	bool bunishing = false;

	while (si3m.begin_frame())
	{
		const float fSpeed = 0.04f;

		if (key.pushing(DIK_UP))
		{
			camera.y(camera.y() + fSpeed);
			camera.look_y(camera.y());
		}
		if (key.pushing(DIK_DOWN))
		{
			camera.y(camera.y() - fSpeed);
			camera.look_y(camera.y());
		}

		if (key.pushing(DIK_Q))
		{
			fRadius += fSpeed;
		}
		if (key.pushing(DIK_W))
		{
			fRadius -= fSpeed;
		}

#if 1
		if (key.pushing(DIK_O) || key.pushing(DIK_LEFT))
		{
			fRadian += fRotSpeed;
		}
		if (key.pushing(DIK_P) || key.pushing(DIK_RIGHT))
		{
			fRadian -= fRotSpeed;
		}
		camera.x(camera.look_x() - cos(fRadian)*fRadius);
		camera.z(camera.look_z() + sin(fRadian)*fRadius);
#endif

		if (key.pushed(DIK_U))
		{
		//	miku.mask_index -= 1;
		}
		if (key.pushed(DIK_J))
		{
		//	miku.mask_index += 1;
		}

		if (key.pushed(DIK_RETURN))
		{
			yuuki.seek_first();
		}

		si3m.register_display_object(yuuki);

		si3m.show();


	}

	delete si3mp;

	return 0;
}



