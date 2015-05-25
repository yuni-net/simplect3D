#if 1

#include <utility.h>
#include <sphere_data.h>

static const WORD SPHERE_FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

namespace si3
{
	sphere_data::sphere_data()
	{
		construct();
	}
	sphere_data::sphere_data(LPDIRECT3DDEVICE9 device, const char * path, float smoothness, float radius)
	{
		construct();
		load(device, path, smoothness, radius);
	}

	void sphere_data::load(LPDIRECT3DDEVICE9 device, const char * path, float smoothness, float radius)
	{
		this->device = device;

		release();

		D3DXCreateSphere(
			device,
			radius,
			static_cast<UINT>(20 * smoothness),
			static_cast<UINT>(14 * smoothness),
			&mesh,
			NULL);


		// テクスチャ作成
		HRESULT hr = D3DXCreateTextureFromFileEx(
			device,
			path,
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT_NONPOW2,
			0,
			0,
			D3DFMT_UNKNOWN,
			D3DPOOL_MANAGED,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			0,
			NULL,
			NULL,
			&texture);
		if (FAILED(hr)) return;

	}


	void sphere_data::draw(const D3DXMATRIX & world_mat) const
	{
		// ワールド変換行列設定
		device->SetTransform(D3DTS_WORLD, &world_mat);

		// 頂点フォーマット設定
		device->SetFVF(SPHERE_FVF);


		device->SetTexture(0, texture);

		mesh->DrawSubset(0);

	}


	sphere_data::~sphere_data()
	{
		release();
	}

	void sphere_data::construct()
	{
		device = nullptr;
		mesh = nullptr;
		texture = nullptr;
	}
	void sphere_data::release()
	{
		saferelease(mesh);
		saferelease(texture);
	}


}

#endif