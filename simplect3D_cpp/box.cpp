#if 0

#include <box.h>

namespace si3
{
	void box::x(float value)
	{
		x_ = value;
	}
	float box::x() const
	{
		return x_;
	}

	void box::y(float value)
	{
		y_ = value;
	}
	float box::y() const
	{
		return y_;
	}

	void box::z(float value)
	{
		z_ = value;
	}
	float box::z() const
	{
		return z_;
	}

	void box::radius(float value)
	{
		radius_ = value;
	}
	float box::radius() const
	{
		return radius_;
	}

#if 0
	void box::red(float value)
	{
		red_ = value;
	}
	float box::red() const
	{
		return red_;
	}

	void box::green(float value)
	{
		green_ = value;
	}
	float box::green() const
	{
		return green_;
	}

	void box::blue(float value)
	{
		blue_ = value;
	}
	float box::blue() const
	{
		return blue_;
	}
#endif


	void box::draw_no_alpha() const
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

		boxd->draw(world_mat);
	}
	void box::draw_alpha() const
	{
		// todo
		// Œ»İ–¢‘Î‰
	}

}

#endif