#if 1

#include <si3_Box.h>

namespace si3
{
	void Box::set_box_data(const BoxData & boxd)
	{
		this->boxd = &boxd;
	}

	void Box::x(float value)
	{
		x_ = value;
	}
	float Box::x() const
	{
		return x_;
	}

	void Box::y(float value)
	{
		y_ = value;
	}
	float Box::y() const
	{
		return y_;
	}

	void Box::z(float value)
	{
		z_ = value;
	}
	float Box::z() const
	{
		return z_;
	}


	float Box::rot_x() const
	{
		return rot_x_;
	}
	void Box::rot_x(float value)
	{
		rot_x_ = value;
	}

	float Box::rot_y() const
	{
		return rot_y_;
	}
	void Box::rot_y(float value)
	{
		rot_y_ = value;
	}

	float Box::rot_z() const
	{
		return rot_z_;
	}
	void Box::rot_z(float value)
	{
		rot_z_ = value;
	}


	float Box::red() const
	{
		return red_;
	}
	void Box::red(float value)
	{
		red_ = value;
	}

	float Box::green() const
	{
		return green_;
	}
	void Box::green(float value)
	{
		green_ = value;
	}

	float Box::blue() const
	{
		return blue_;
	}
	void Box::blue(float value)
	{
		blue_ = value;
	}

	float Box::alpha() const
	{
		return alpha_;
	}
	void Box::alpha(float value)
	{
		alpha_ = value;
	}


	void Box::draw_no_alpha() const
	{
		if (alpha() < 1.0f)
		{
			return;
		}

		auto device = boxd->get_device();

		// Zバッファを更新する
		device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		// αテスト禁止
		device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

		// 半透明表示禁止(αブレンディング無効化)
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

		Matrix rotate_mat =
			Matrix().rotate_y(rot_y()) *
			Matrix().rotate_x(rot_x()) *
			Matrix().rotate_z(rot_z());

		Matrix parallel_mat;
		parallel_mat.parallel(x(), y(), z());

		Matrix world_mat;
		world_mat = rotate_mat*parallel_mat;

		// ワールド変換行列設定
		device->SetTransform(D3DTS_WORLD, world_mat.dxpointer());

		D3DMATERIAL9 material = {
			{ red(), green(), blue(), alpha() },
			{ red(), green(), blue(), alpha() },
			{ 0.0f, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 0.0f },
			1.0f
		};

		//マテリアル設定
		device->SetMaterial(&material);


		boxd->draw();
	}
	void Box::draw_alpha() const
	{
		if (alpha() >= 1.0f)
		{
			return;
		}

		auto device = boxd->get_device();

		// Zバッファを更新しない
		device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		// 半透明表示を可能にする(αブレンディング有効化)
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		// 完全透明部分を書き込まない(αテスト有効化)
		device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		device->SetRenderState(D3DRS_ALPHAREF, 0);
		device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		Matrix rotate_mat =
			Matrix().rotate_y(rot_y()) *
			Matrix().rotate_x(rot_x()) *
			Matrix().rotate_z(rot_z());

		Matrix parallel_mat;
		parallel_mat.parallel(x(), y(), z());

		Matrix world_mat;
		world_mat = rotate_mat*parallel_mat;

		// ワールド変換行列設定
		device->SetTransform(D3DTS_WORLD, world_mat.dxpointer());

		D3DMATERIAL9 material = {
			{ red(), green(), blue(), alpha() },
			{ red(), green(), blue(), alpha() },
			{ 0.0f, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 0.0f },
			1.0f
		};

		//マテリアル設定
		device->SetMaterial(&material);


		boxd->draw();
	}

}

#endif