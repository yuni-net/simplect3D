/*
** GM31 No.01
** v2.1.0
*/

#include <simplect3D.h>
#include <memory>

class Morf :public si3::DisplayObject
{
public:
	si3::Matrix matrix;
	bool culling;
	si3::MorfData morfd;

	Morf(const std::string & before, const std::string & after) :
		morfd(before, after)
	{
		// Nothing
	}

	// @Override
	void draw_no_alpha() const
	{
		morfd.draw_no_alpha(matrix, culling);
	}

	// @Override
	void draw_alpha() const
	{
		morfd.draw_alpha(matrix);
	}
};

int WINAPI WinMain(HINSTANCE hI, HINSTANCE hP, LPSTR lpC, int nC)
{
	fw::xrandom_init(static_cast<unsigned long>(time(NULL)));

	if (si3::Manager::init(1024, 768, std::string("「マジで大変身」NH AT13A222 05 小野正貴")) == false)
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

	float fRadian = -3.14159265f / 2.0f - 0.5f;
	const float fRotSpeed = 0.08f;
	float fRadius = 14.0f;

	camera.x(0.0f);
	camera.y(18.0f);
	camera.z(-fRadius);

	camera.look_x(0.0f);
	camera.look_y(12.0f);
	camera.look_z(0.0f);


	Morf morf(std::string("data/yuuki/yuuki_before.pmd"), std::string("data/Lat_Miku/Miku_newface.pmd"));
	morf.matrix.parallel(3.0f, 0.0f, 0.0f);
	morf.culling = false;


	const float morf_speed = 0.004f;
	float morf_radian = 0.0f;

	while (si3::Manager::begin_frame())
	{


		camera.x(camera.look_x() - cos(fRadian)*fRadius);
		camera.z(camera.look_z() + sin(fRadian)*fRadius);


		morf_radian += morf_speed;
		if (morf_radian > 2*si3::pi)
		{
			morf_radian -= 2*si3::pi;
		}
		const float morf_percent = fabsf(cosf(morf_radian));
		morf.morfd.update(morf_percent);


		si3::Manager::register_display_object(morf);

		si3::Manager::show();


	}


	return 0;
}



