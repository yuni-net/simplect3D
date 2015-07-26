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
	Skydome::Skydome(const Manager & manageri, const TCHAR * path)
	{
		construct();
		load(manageri, path);
	}
	void Skydome::load(const Manager & manageri, const TCHAR * path)
	{
		managerp = &manageri;
		sphered.load(manageri.get_dxdevice(), path, 0.1f);
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

		const auto & device = managerp->get_dxdevice();

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
		managerp = nullptr;
		loaded = false;
	}

	void Skydome::release()
	{
		// Nothing
	}
}