#if 1

#include <box.h>

namespace si3
{
	void box::set_box_data(const box_data & boxd)
	{
		this->boxd = &boxd;
	}

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


	float box::rot_x() const
	{
		return rot_x_;
	}
	void box::rot_x(float value)
	{
		rot_x_ = value;
	}

	float box::rot_y() const
	{
		return rot_y_;
	}
	void box::rot_y(float value)
	{
		rot_y_ = value;
	}

	float box::rot_z() const
	{
		return rot_z_;
	}
	void box::rot_z(float value)
	{
		rot_z_ = value;
	}


	float box::red() const
	{
		return red_;
	}
	void box::red(float value)
	{
		red_ = value;
	}

	float box::green() const
	{
		return green_;
	}
	void box::green(float value)
	{
		green_ = value;
	}

	float box::blue() const
	{
		return blue_;
	}
	void box::blue(float value)
	{
		blue_ = value;
	}

	float box::alpha() const
	{
		return alpha_;
	}
	void box::alpha(float value)
	{
		alpha_ = value;
	}


	void box::draw_no_alpha() const
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

		matrix rotate_mat =
			matrix().rotate_y(rot_y()) *
			matrix().rotate_x(rot_x()) *
			matrix().rotate_z(rot_z());

		matrix parallel_mat;
		parallel_mat.parallel(x(), y(), z());

		matrix world_mat;
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
	void box::draw_alpha() const
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

		matrix rotate_mat =
			matrix().rotate_y(rot_y()) *
			matrix().rotate_x(rot_x()) *
			matrix().rotate_z(rot_z());

		matrix parallel_mat;
		parallel_mat.parallel(x(), y(), z());

		matrix world_mat;
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