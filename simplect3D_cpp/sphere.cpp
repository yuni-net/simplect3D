#if 1

#include <si3_Sphere.h>
#include <si3_Manager.h>
#include <si3_Matrix.h>

namespace si3
{
	void Sphere::x(float value)
	{
		x_ = value;
	}
	float Sphere::x() const
	{
		return x_;
	}

	void Sphere::y(float value)
	{
		y_ = value;
	}
	float Sphere::y() const
	{
		return y_;
	}

	void Sphere::z(float value)
	{
		z_ = value;
	}
	float Sphere::z() const
	{
		return z_;
	}

	float Sphere::radius() const
	{
		return radius_;
	}

#if 1
	void Sphere::red(float value)
	{
		red_ = value;
	}
	float Sphere::red() const
	{
		return red_;
	}

	void Sphere::green(float value)
	{
		green_ = value;
	}
	float Sphere::green() const
	{
		return green_;
	}

	void Sphere::blue(float value)
	{
		blue_ = value;
	}
	float Sphere::blue() const
	{
		return blue_;
	}

	void Sphere::alpha(float value)
	{
		alpha_ = value;
	}
	float Sphere::alpha() const
	{
		return alpha_;
	}
#endif

	void Sphere::construct()
	{
		sphered = nullptr;
		ownership = false;
	}

	Sphere::Sphere()
	{
		construct();
	}

	Sphere::Sphere(const char * path, float smoothness, float radius)
	{
		construct();
		create(path, smoothness, radius);
	}

	void Sphere::create(const char * path, float smoothness, float radius)
	{
		device = si3::Manager::get_dxdevice();
		radius_ = radius;
		sphered = new SphereData(path, smoothness, radius);
		ownership = true;
	}



	Sphere::~Sphere()
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

	void Sphere::rot_x(float value)
	{
		rot_x_ = value;
	}
	float Sphere::rot_x() const
	{
		return rot_x_;
	}

	void Sphere::rot_y(float value)
	{
		rot_y_ = value;
	}
	float Sphere::rot_y() const
	{
		return rot_y_;
	}

	void Sphere::rot_z(float value)
	{
		rot_z_ = value;
	}
	float Sphere::rot_z() const
	{
		return rot_z_;
	}



	void Sphere::draw_no_alpha() const
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


		Matrix rot_mat =
			Matrix().rotate_y(rot_y()) *
			Matrix().rotate_x(rot_x()) *
			Matrix().rotate_z(rot_z());

		Matrix para_mat;
		para_mat.parallel(x(), y(), z());

		Matrix world_mat = rot_mat*para_mat;


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
	void Sphere::draw_alpha() const
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


		Matrix rot_mat =
			Matrix().rotate_y(rot_y()) *
			Matrix().rotate_x(rot_x()) *
			Matrix().rotate_z(rot_z());

		Matrix para_mat;
		para_mat.parallel(x(), y(), z());

		Matrix world_mat = rot_mat*para_mat;


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