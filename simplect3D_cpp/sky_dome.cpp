#include "si3_Skydome.h"
#include "si3_Manager.h"
#include "si3_saferelease.h"
#include "si3_Matrix.h"

namespace si3
{
	Skydome::Skydome()
	{
		construct();
	}
	Skydome::Skydome(const TCHAR * path)
	{
		construct();
		load(path);
	}
	void Skydome::load(const TCHAR * path)
	{
		sphered.load(path, 0.1f);
		loaded = true;
	}


	void Skydome::x(float value)
	{
		x_ = value;
	}
	float Skydome::x() const
	{
		return x_;
	}
	void Skydome::y(float value)
	{
		y_ = value;
	}
	float Skydome::y() const
	{
		return y_;
	}
	void Skydome::z(float value)
	{
		z_ = value;
	}
	float Skydome::z() const
	{
		return z_;
	}

	void Skydome::radius(float value)
	{
		radius_ = value;
	}
	float Skydome::radius() const
	{
		return radius_;
	}



	Skydome::~Skydome()
	{
		release();
	}

	// @override
	void Skydome::draw_no_alpha() const
	{
		if (loaded == false)
		{
			return;
		}

		const auto & device = si3::Manager::get_dxdevice();

		// Zバッファを更新しない
		device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);



		Matrix scale_mat;
		scale_mat.scale(radius(), radius(), radius());

		Matrix parallel_mat;
		parallel_mat.x(x());
		parallel_mat.y(y());
		parallel_mat.z(z());

		Matrix world_mat = scale_mat * parallel_mat;

		sphered.draw(world_mat.dxmat());
	}

	// @override
	void Skydome::draw_alpha() const
	{
		// Nothing
	}



	void Skydome::construct()
	{
		loaded = false;
	}

	void Skydome::release()
	{
		// Nothing
	}
}