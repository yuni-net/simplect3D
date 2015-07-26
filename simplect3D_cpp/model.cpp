#include <si3_Model.h>
#include <si3_Manager.h>
#include <si3_modeldata.h>

namespace si3
{
	Model::Model(const Manager & manageri, const TCHAR * path)
	{
		construct();
		load(manageri, path);
	}
	Model::Model(const Manager & manageri, const tstring & path)
	{
		construct();
		load(manageri, path);
	}
	Model::Model(const Manager & manageri, const si3::ModelData & modeld)
	{
		construct();
		model_data(manageri, modeld);
	}
	Model::Model(const Manager & manageri, const TCHAR * path, const Coor3 & center)
	{
		construct();
		load(manageri, path, center);
	}


	bool Model::load(const Manager & manageri, const TCHAR * path)
	{
		dxdevice = manageri.get_dxdevice();
		release();
		ownership = true;
		modeld = new ::si3::ModelData(manageri, path);

		return true;
	}
	bool Model::load(const Manager & manageri, const tstring & path)
	{
		return load(manageri, path.c_str());
	}
	void Model::model_data(const Manager & manageri, const si3::ModelData & modeld)
	{
		dxdevice = manageri.get_dxdevice();
		release();
		this->modeld = &modeld;
	}
	bool Model::load(const Manager & manageri, const TCHAR * path, const Coor3 & center)
	{
		dxdevice = manageri.get_dxdevice();
		release();
		ownership = true;
		modeld = new ::si3::ModelData(manageri, path, center);

		return true;
	}


	const si3::ModelData & Model::model_data() const
	{
		return *modeld;
	}


	void Model::x(float value)
	{
		coor.x = value;
	}
	float Model::x() const
	{
		return coor.x;
	}

	void Model::y(float value)
	{
		coor.y = value;
	}
	float Model::y() const
	{
		return coor.y;
	}

	void Model::z(float value)
	{
		coor.z = value;
	}
	float Model::z() const
	{
		return coor.z;
	}


	void Model::radian_x(float value)
	{
		coor.radian_x = value;
	}
	float Model::radian_x() const
	{
		return coor.radian_x;
	}

	void Model::radian_y(float value)
	{
		coor.radian_y = value;
	}
	float Model::radian_y() const
	{
		return coor.radian_y;
	}

	void Model::radian_z(float value)
	{
		coor.radian_z = value;
	}
	float Model::radian_z() const
	{
		return coor.radian_z;
	}

	void Model::affine(const Matrix & world_mat)
	{
		this->world_mat = world_mat;
		computed_world_mat = true;
	}
	const Matrix & Model::affine() const
	{
		compute_world_mat();

		return world_mat;
	}

	void Model::culling(bool enable)
	{
		this->is_culling_on = enable;
	}


	/*
	* x,y,zの回転の順序を設定する。
	* デフォルトでは y→x→z の順
	*/
	void Model::set_rot_mode(RotMode::ERotMode first, RotMode::ERotMode second, RotMode::ERotMode third)
	{
		first_rot_mode = first;
		second_rot_mode = second;
		third_rot_mode = third;
	}




	Model::Model()
	{
		construct();
	}

	Model::~Model()
	{
		release();
	}

	// @override
	void Model::draw_no_alpha() const
	{
		compute_world_mat();
		computed_world_mat = false;

		if (is_culling_on)
		{
			dxdevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		}
		else
		{
			dxdevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		}

		this->modeld->draw_no_alpha(world_mat, is_culling_on);
	}

	// @override
	void Model::draw_alpha() const
	{
		dxdevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		this->modeld->draw_alpha(world_mat);
	}

	void Model::construct()
	{
		ownership = false;
		modeld = nullptr;

		coor.radian_x = 0.0f;
		coor.radian_y = 0.0f;
		coor.radian_z = 0.0f;

		first_rot_mode = RotMode::y;
		second_rot_mode = RotMode::x;
		third_rot_mode = RotMode::z;

		computed_world_mat = false;
		is_culling_on = true;
	}

	void Model::release()
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

	void Model::set_rot_mat(Matrix & rot_mat, RotMode::ERotMode rot_mode_) const
	{
		if (rot_mode_ == RotMode::x)
		{
			rot_mat.rotate_x(coor.radian_x);
		}

		if (rot_mode_ == RotMode::y)
		{
			rot_mat.rotate_y(coor.radian_y);
		}

		if (rot_mode_ == RotMode::z)
		{
			rot_mat.rotate_z(coor.radian_z);
		}
	}

	void Model::compute_world_mat() const
	{
		if (computed_world_mat)
		{
			return;
		}

		Matrix first_rot;
		set_rot_mat(first_rot, first_rot_mode);

		Matrix second_rot;
		set_rot_mat(second_rot, second_rot_mode);

		Matrix third_rot;
		set_rot_mat(third_rot, third_rot_mode);

		world_mat =
			first_rot *
			second_rot *
			third_rot *
			Matrix().parallel(coor.x, coor.y, coor.z);
	}

}