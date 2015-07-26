#include "si3_SkydomeMini.h"
#include "si3_Manager.h"
#include "si3_saferelease.h"
#include "si3_Matrix.h"

namespace si3
{
	Skydome_mini::Skydome_mini()
	{
		construct();
	}
	Skydome_mini::Skydome_mini(const Manager & manageri, const TCHAR * path)
	{
		construct();
		load(manageri, path);
	}
	void Skydome_mini::load(const Manager & manageri, const TCHAR * path)
	{
		managerp = &manageri;
		sphered.load(manageri.get_dxdevice(), path, 0.1f);
		loaded = true;
	}


	Skydome_mini::~Skydome_mini()
	{
		release();
	}

	// @override
	void Skydome_mini::draw_no_alpha() const
	{
		if (loaded == false)
		{
			return;
		}

		const auto & device = managerp->get_dxdevice();


		// αテスト禁止
		device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

		// 半透明表示禁止(αブレンディング無効化)
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

		// Zバッファを更新しない
		device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);


		const si3::Camera & camera = managerp->camera();
		float near_clip = camera.near_clip()+0.1f;

		Matrix scale_mat;
		scale_mat.scale(near_clip, near_clip, near_clip);

		Matrix parallel_mat;
		parallel_mat.x(camera.x());
		parallel_mat.y(camera.y());
		parallel_mat.z(camera.z());

		Matrix world_mat = scale_mat * parallel_mat;

		//マテリアル設定
		static const D3DMATERIAL9 material = {
			{ 1.0f, 1.0f, 1.0f, 1.0f },		// Diffuse
			{ 1.0f, 1.0f, 1.0f, 1.0f },		// Ambient
			{ 0.0f, 0.0f, 0.0f, 0.0f },		// Specular
			{ 0.0f, 0.0f, 0.0f, 0.0f },		// Emissive
			0.0f };
		device->SetMaterial(&material);

		sphered.draw(world_mat.dxmat());
	}

	// @override
	void Skydome_mini::draw_alpha() const
	{
		// Nothing
	}



	void Skydome_mini::construct()
	{
		managerp = nullptr;
		loaded = false;
	}

	void Skydome_mini::release()
	{
		// Nothing
	}
}