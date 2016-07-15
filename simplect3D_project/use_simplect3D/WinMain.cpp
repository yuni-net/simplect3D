/*
** GM31 No.01
** v2.1.0
*/

#include <simplect3D.h>



int WINAPI WinMain(HINSTANCE hI, HINSTANCE hP, LPSTR lpC, int nC)
{
	fw::xrandom_init(static_cast<unsigned long>(time(NULL)));

	if (si3::Manager::init(960, 600) == false)
	{
		fw::popup("failed");
		return -1;
	}

	const si3::Key & key = si3::Manager::key();
	si3::Camera & camera = si3::Manager::camera();
	const si3::Mouse & mouse = si3::Manager::mouse();
	auto & displaym = si3::Manager::display_manager();

	displaym.context_color(1.0f, 1.0f, 1.0f);

	auto & skydome = si3::Manager::display_manager().get_skydome();
	skydome.load("data/orange.jpg");

	camera.x(0.0f);
	camera.y(00.0f);
	camera.z(-32.0f);

	camera.look_x(0.0f);
	camera.look_y(0.0f);
	camera.look_z(0.0f);

	si3::Triangle triangle(
		si3::Coor3(-10.0f, -10.0f, 0.0f),
		si3::Coor3(10.0f,-10.0f,0.0f),
		si3::Coor3(10.0f,10.0f,0.0f));
	triangle.x = 0.0f;
	triangle.y = 0.0f;
	triangle.z = 0.0f;


	while (si3::Manager::begin_frame())
	{
		si3::Manager::register_display_object(triangle);

		si3::Manager::show();


	}


	return 0;
}



