/*
** GM31 No.01
** v2.1.0
*/

#include <simplect3D.h>



int WINAPI WinMain(HINSTANCE hI, HINSTANCE hP, LPSTR lpC, int nC)
{
	fw::xrandom_init(static_cast<unsigned long>(time(NULL)));

	si3::manager * si3mp = new si3::manager();
	si3::manager & si3m = *si3mp;
	if (si3m.init() == false)
	{
		fw::popup("failed");
		return -1;
	}

	const si3::key & key = si3m.key();
	si3::camera & camera = si3m.camera();
	const si3::mouse & mouse = si3m.mouse();
	auto & displaym = si3m.display_manager();

	displaym.context_color(1.0f, 1.0f, 1.0f);

	auto & skydome = si3m.display_manager().get_skydome();
	skydome.load(si3m, "data/orange.jpg");

	camera.x(0.0f);
	camera.y(10.0f);
	camera.z(-32.0f);

	camera.look_x(0.0f);
	camera.look_y(10.0f);
	camera.look_z(0.0f);

	si3::model_data miku_data;
	miku_data.load(si3m, "data/Lat_Miku/nanase_pose.pmd");

	si3::model miku;
	miku.model_data(si3m, miku_data);
	miku.x(0.0f);
	miku.y(0.0f);
	miku.z(0.0f);

	si3::bunisher miku_bunish;

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
			miku_bunish.init(si3m, miku);
			bunishing = true;
		}

		if (bunishing)
		{
			si3m.register_display_object(miku_bunish);
		}
		else
		{
			si3m.register_display_object(miku);
		}


		si3m.show();


	}

	delete si3mp;

	return 0;
}



