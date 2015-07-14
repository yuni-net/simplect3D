#include <model.h>
#include <Manager.h>
#include <model_data.h>

namespace si3
{
	model::model(const Manager & manageri, const TCHAR * path)
	{
		construct();
		load(manageri, path);
	}
	model::model(const Manager & manageri, const tstring & path)
	{
		construct();
		load(manageri, path);
	}
	model::model(const Manager & manageri, const si3::model_data & modeld)
	{
		construct();
		model_data(manageri, modeld);
	}
	model::model(const Manager & manageri, const TCHAR * path, const coor3 & center)
	{
		construct();
		load(manageri, path, center);
	}


	bool model::load(const Manager & manageri, const TCHAR * path)
	{
		dxdevice = manageri.get_dxdevice();
		release();
		ownership = true;
		modeld = new ::si3::model_data(manageri, path);

		return true;
	}
	bool model::load(const Manager & manageri, const tstring & path)
	{
		return load(manageri, path.c_str());
	}
	void model::model_data(const Manager & manageri, const si3::model_data & modeld)
	{
		dxdevice = manageri.get_dxdevice();
		release();
		this->modeld = &modeld;
	}
	bool model::load(const Manager & manageri, const TCHAR * path, const coor3 & center)
	{
		dxdevice = manageri.get_dxdevice();
		release();
		ownership = true;
		modeld = new ::si3::model_data(manageri, path, center);

		return true;
	}


	const si3::model_data & model::model_data() const
	{
		return *modeld;
	}


	void model::x(float value)
	{
		coor.x = value;
	}
	float model::x() const
	{
		return coor.x;
	}

	void model::y(float value)
	{
		coor.y = value;
	}
	float model::y() const
	{
		return coor.y;
	}

	void model::z(float value)
	{
		coor.z = value;
	}
	float model::z() const
	{
		return coor.z;
	}


	void model::radian_x(float value)
	{
		coor.radian_x = value;
	}
	float model::radian_x() const
	{
		return coor.radian_x;
	}

	void model::radian_y(float value)
	{
		coor.radian_y = value;
	}
	float model::radian_y() const
	{
		return coor.radian_y;
	}

	void model::radian_z(float value)
	{
		coor.radian_z = value;
	}
	float model::radian_z() const
	{
		return coor.radian_z;
	}

	void model::affine(const matrix & world_mat)
	{
		this->world_mat = world_mat;
		bComputedWorldMat = true;
	}
	const matrix & model::affine() const
	{
		compute_world_mat();

		return world_mat;
	}

	void model::culling(bool bCulling)
	{
		this->bCulling = bCulling;
	}


	/*
	* x,y,zの回転の順序を設定する。
	* デフォルトでは y→x→z の順
	*/
	void model::set_rot_mode(rot_mode::ERotMode first, rot_mode::ERotMode second, rot_mode::ERotMode third)
	{
		first_rot_mode = first;
		second_rot_mode = second;
		third_rot_mode = third;
	}




	model::model()
	{
		construct();
	}

	model::~model()
	{
		release();
	}

	// @override
	void model::draw_no_alpha() const
	{
		compute_world_mat();
		bComputedWorldMat = false;

		if (bCulling)
		{
			dxdevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		}
		else
		{
			dxdevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		}

		this->modeld->draw_no_alpha(world_mat, bCulling);
	}

	// @override
	void model::draw_alpha() const
	{
		dxdevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		this->modeld->draw_alpha(world_mat);
	}

	void model::construct()
	{
		ownership = false;
		modeld = nullptr;

		coor.radian_x = 0.0f;
		coor.radian_y = 0.0f;
		coor.radian_z = 0.0f;

		first_rot_mode = rot_mode::y;
		second_rot_mode = rot_mode::x;
		third_rot_mode = rot_mode::z;

		bComputedWorldMat = false;
		bCulling = true;
	}

	void model::release()
	{
		if (ownership)
		{
			if (modeld)
			{
				delete modeld;
				modeld = nullptr;
			}
		}

		ownership = false;
	}

	void model::set_rot_mat(matrix & rot_mat, rot_mode::ERotMode rot_mode_) const
	{
		if (rot_mode_ == rot_mode::x)
		{
			rot_mat.rotate_x(coor.radian_x);
		}

		if (rot_mode_ == rot_mode::y)
		{
			rot_mat.rotate_y(coor.radian_y);
		}

		if (rot_mode_ == rot_mode::z)
		{
			rot_mat.rotate_z(coor.radian_z);
		}
	}

	void model::compute_world_mat() const
	{
		if (bComputedWorldMat)
		{
			return;
		}

		matrix first_rot;
		set_rot_mat(first_rot, first_rot_mode);

		matrix second_rot;
		set_rot_mat(second_rot, second_rot_mode);

		matrix third_rot;
		set_rot_mat(third_rot, third_rot_mode);

		world_mat =
			first_rot *
			second_rot *
			third_rot *
			matrix().parallel(coor.x, coor.y, coor.z);
	}

}