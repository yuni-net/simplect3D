/*
** GM31 No.01
** v2.1.0
*/

#include <simplect3D.h>

struct soundID
{
	enum
	{
		bgm,
		num
	};
};

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

	displaym.context_color(0.0f, 0.0f, 0.0f);

	camera.x(0.0f);
	camera.y(0.0f);
	camera.z(-32.0f);

	camera.look_x(0.0f);
	camera.look_y(0.0f);
	camera.look_z(0.0f);

	si3::sky_dome skydome;
	skydome.load(si3m, "data/orange.jpg");
	skydome.x(0.0f);
	skydome.y(0.0f);
	skydome.z(0.0f);
	skydome.radius(60.0f);

//	skydome.load(si3m, "data/sky.jpg");

//	si3::model_data miku_data(si3m, "data/Lat_Miku/miku_newface.pmd");
	si3::model_data miku_data(si3m, "data/nanase_pose.pmd");

	si3::model miku(si3m, miku_data);
//	si3::model miku(si3m, "data/Lat_Miku/miku_alpha.pmd");
	miku.x(0.0f);
	miku.y(0.0f);
	miku.z(0.0f);



	si3::bunisher mikusher;

	float fRadian = -3.14159265f/2.0f;
	const float fRotSpeed = 0.08f;
	float fRadius = 8.0f;

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

		if (key.pushed(DIK_SPACE))
		{
			bunishing = true;
			mikusher.init(si3m, miku);
		}
		if (key.pushed(DIK_RETURN))
		{
			bunishing = false;
		}

		if (bunishing)
		{
			mikusher.bunish();
			si3m.register_display_object(mikusher);
		}
		else
		{
			si3m.register_display_object(miku);
		}

		si3m.register_display_object(skydome);

		si3m.show();


	}

	delete si3mp;

	return 0;
}



#if 0

#include <simplect3D.h>

const float c_fPi = 3.14159265f;

int WINAPI WinMain(HINSTANCE hI, HINSTANCE hP, LPSTR lpC, int nC)
{
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

	camera.x(0.0f);
	camera.y(16.0f);
	camera.z(-16.0f);

	si3::model war;
	war.load(si3m, "data/war.x");
	war.x(0.0f);
	war.y(6.0f);
	war.z(0.0f);
	war.radian_y(c_fPi / 2.0f);

	si3::leaf baseland;
	baseland.load(si3m, "data/land.bmp");
	baseland.x(0.0f);
	baseland.y(0.0f);
	baseland.z(0.0f);
	baseland.rot_x(c_fPi / 2.0f);

	si3::anime_data bombd;
	bombd.load(si3m, "data/bomb_anime.png", 400.0f, 960 / 5, 576 / 3, 15);

	si3::anime bomb;
	bomb.setblend_normal();
	bomb.anime_data(si3m, bombd);
	bomb.x(-16.0f);
	bomb.y(6.0f);
	bomb.z(6.0f);
	bomb.scale(0.06f);
	bomb.setblend_alpha();

	si3::leaf shadow(si3m, "data/shadow.bmp");
	shadow.setblend_sub();
	shadow.y(0.01f);
	shadow.rot_x(3.14159265f / 2.0f);
	shadow.scale(0.1f);

	si3::billboard end(si3m, "data/end.bmp");
	end.scale(0.04f);
	end.x(8.0f);
	end.y(6.0f);
	end.z(0.0f);


	while (si3m.begin_frame())
	{
		// player ***********************************************

		const float c_fRotSpeed = 0.06f;

		if (key.pushing(DIK_RIGHT))
		{
			war.radian_y(war.radian_y() + c_fRotSpeed);
		}

		if (key.pushing(DIK_LEFT))
		{
			war.radian_y(war.radian_y() - c_fRotSpeed);
		}


		if (key.pushing(DIK_UP))
		{
			const float c_fSpeed = 0.2f;

			float fVX = -cos(war.radian_y())*c_fSpeed;
			float fVZ = sin(war.radian_y())*c_fSpeed;

			war.x(war.x() + fVX);
			war.z(war.z() + fVZ);
		}

		bomb.animation();

		si3m.register_display_object(war);

		// *****************************************************


		shadow.x(war.x());
		shadow.z(war.z());
		si3m.register_display_object(shadow);

		// camera **********************************************

		float fRadian = war.radian_y() + c_fPi;
		const float c_fRadius = 26.0f;
		camera.x(war.x() - cos(fRadian)*c_fRadius);
		camera.z(war.z() + sin(fRadian)*c_fRadius);

		camera.look_x(war.x());
		camera.look_y(war.y());
		camera.look_z(war.z());

		// *****************************************************

		si3m.register_display_object(bomb);
		si3m.register_display_object(baseland);
		si3m.register_display_object(end);


		si3m.show();


	}

	delete si3mp;

	return 0;
}

#endif