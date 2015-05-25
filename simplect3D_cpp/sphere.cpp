#if 1

#include <sphere.h>
#include <manager.h>
#include <matrix.h>

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

	float sphere::radius() const
	{
		return radius_;
	}

#if 1
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

	void sphere::alpha(float value)
	{
		alpha_ = value;
	}
	float sphere::alpha() const
	{
		return alpha_;
	}
#endif

	void sphere::construct()
	{
		sphered = nullptr;
		ownership = false;
	}

	sphere::sphere()
	{
		construct();
	}

	sphere::sphere(const manager & si3m, const char * path, float smoothness, float radius)
	{
		construct();
		create(si3m, path, smoothness, radius);
	}

	void sphere::create(const manager & si3m, const char * path, float smoothness, float radius)
	{
		device = si3m.get_dxdevice();
		radius_ = radius;
		sphered = new sphere_data(si3m.get_dxdevice(), path, smoothness, radius);
		ownership = true;
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

	void sphere::rot_x(float value)
	{
		rot_x_ = value;
	}
	float sphere::rot_x() const
	{
		return rot_x_;
	}

	void sphere::rot_y(float value)
	{
		rot_y_ = value;
	}
	float sphere::rot_y() const
	{
		return rot_y_;
	}

	void sphere::rot_z(float value)
	{
		rot_z_ = value;
	}
	float sphere::rot_z() const
	{
		return rot_z_;
	}



	void sphere::draw_no_alpha() const
	{
		if (alpha() < 1.0f)
		{
			return;
		}

		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		//device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		// Zバッファを更新する
		device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		// αテスト禁止
		device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

		// 半透明表示禁止(αブレンディング無効化)
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

		// マテリアルとテクスチャのrgba情報を掛け合わせる設定
		device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
		device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);


		matrix rot_mat =
			matrix().rotate_y(rot_y()) *
			matrix().rotate_x(rot_x()) *
			matrix().rotate_z(rot_z());

		matrix para_mat;
		para_mat.parallel(x(), y(), z());

		matrix world_mat = rot_mat*para_mat;


		//マテリアル設定
		const D3DMATERIAL9 material = {
			{ red(), green(), blue(), alpha() },		// Diffuse
			{ red(), green(), blue(), alpha() },		// Ambient
			{ 0.0f, 0.0f, 0.0f, 0.0f },		// Specular
			{ 0.0f, 0.0f, 0.0f, 0.0f },		// Emissive
			1.0f };
		device->SetMaterial(&material);

		sphered->draw(world_mat.dxmat());
	}
	void sphere::draw_alpha() const
	{
		if (alpha() >= 1.0f)
		{
			return;
		}

		// マテリアルとテクスチャのrgba情報を掛け合わせる設定
		device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
		device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);

		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		// 半透明表示を可能にする(αブレンディング有効化)
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		// 完全透明部分を書き込まない(αテスト有効化)
		device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		device->SetRenderState(D3DRS_ALPHAREF, 0);
		device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		// Zバッファを更新しない
		device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);


		matrix rot_mat =
			matrix().rotate_y(rot_y()) *
			matrix().rotate_x(rot_x()) *
			matrix().rotate_z(rot_z());

		matrix para_mat;
		para_mat.parallel(x(), y(), z());

		matrix world_mat = rot_mat*para_mat;


		//マテリアル設定
		const D3DMATERIAL9 material = {
			{ red(), green(), blue(), alpha() },		// Diffuse
			{ red(), green(), blue(), alpha() },		// Ambient
			{ 0.0f, 0.0f, 0.0f, 0.0f },		// Specular
			{ 0.0f, 0.0f, 0.0f, 0.0f },		// Emissive
			1.0f };
		device->SetMaterial(&material);


		sphered->draw(world_mat.dxmat());
	}

}

#endif