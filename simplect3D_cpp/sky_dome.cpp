#include "sky_dome.h"
#include "Manager.h"
#include "saferelease.h"
#include "matrix.h"

namespace si3
{
	sky_dome::sky_dome()
	{
		construct();
	}
	sky_dome::sky_dome(const Manager & manageri, const TCHAR * path)
	{
		construct();
		load(manageri, path);
	}
	void sky_dome::load(const Manager & manageri, const TCHAR * path)
	{
		managerp = &manageri;
		sphered.load(manageri.get_dxdevice(), path, 0.1f);
		loaded = true;
	}


	void sky_dome::x(float value)
	{
		x_ = value;
	}
	float sky_dome::x() const
	{
		return x_;
	}
	void sky_dome::y(float value)
	{
		y_ = value;
	}
	float sky_dome::y() const
	{
		return y_;
	}
	void sky_dome::z(float value)
	{
		z_ = value;
	}
	float sky_dome::z() const
	{
		return z_;
	}

	void sky_dome::radius(float value)
	{
		radius_ = value;
	}
	float sky_dome::radius() const
	{
		return radius_;
	}



	sky_dome::~sky_dome()
	{
		release();
	}

	// @override
	void sky_dome::draw_no_alpha() const
	{
		if (loaded == false)
		{
			return;
		}

		const auto & device = managerp->get_dxdevice();

		// Zバッファを更新しない
		device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);



		matrix scale_mat;
		scale_mat.scale(radius(), radius(), radius());

		matrix parallel_mat;
		parallel_mat.x(x());
		parallel_mat.y(y());
		parallel_mat.z(z());

		matrix world_mat = scale_mat * parallel_mat;

		sphered.draw(world_mat.dxmat());
	}

	// @override
	void sky_dome::draw_alpha() const
	{
		// Nothing
	}



	void sky_dome::construct()
	{
		managerp = nullptr;
		loaded = false;
	}

	void sky_dome::release()
	{
		// Nothing
	}
}