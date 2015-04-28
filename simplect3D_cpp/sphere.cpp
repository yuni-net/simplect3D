#if 1

#include <sphere.h>
#include <manager.h>

namespace si3
{
	void sphere::x(float value)
	{
		x_ = value;
	}
	float sphere::x() const
	{
		return x_;
	}

	void sphere::y(float value)
	{
		y_ = value;
	}
	float sphere::y() const
	{
		return y_;
	}

	void sphere::z(float value)
	{
		z_ = value;
	}
	float sphere::z() const
	{
		return z_;
	}

	void sphere::radius(float value)
	{
		radius_ = value;
	}
	float sphere::radius() const
	{
		return radius_;
	}

#if 0
	void sphere::red(float value)
	{
		red_ = value;
	}
	float sphere::red() const
	{
		return red_;
	}

	void sphere::green(float value)
	{
		green_ = value;
	}
	float sphere::green() const
	{
		return green_;
	}

	void sphere::blue(float value)
	{
		blue_ = value;
	}
	float sphere::blue() const
	{
		return blue_;
	}
#endif





	sphere::sphere(const manager & si3m, float roughness)
	{
		sphered = new sphere_data(si3m.get_dxdevice(), roughness);
		ownership = true;
		radius(1.0f);
	}
	sphere::~sphere()
	{
		if (ownership)
		{
			if (sphered != nullptr)
			{
				delete sphered;
			}
		}

		sphered = nullptr;
		ownership = false;
	}


	void sphere::draw_no_alpha() const
	{
		D3DXMATRIX scale_mat;
		D3DXMatrixScaling(&scale_mat, radius(), radius(), radius());

		D3DXMATRIX rotate_mat;
		//	D3DXMatrixRotationX(&rotate_mat, 3.14159265f);
		D3DXMatrixRotationX(&rotate_mat, 0.0f);

		D3DXMATRIX trans_mat;
		D3DXMatrixTranslation(&trans_mat, x(), y(), z());

		D3DXMATRIX scale_rot;
		D3DXMatrixMultiply(&scale_rot, &scale_mat, &rotate_mat);

		D3DXMATRIX world_mat;
		D3DXMatrixMultiply(&world_mat, &scale_rot, &trans_mat);

		sphered->draw(world_mat);
	}
	void sphere::draw_alpha() const
	{
		// todo
		// Œ»İ–¢‘Î‰
	}

}

#endif