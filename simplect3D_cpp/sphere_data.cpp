#if 1

#include <si3_utility.h>
#include <si3_SphereData.h>
#include <si3_saferelease.h>

static const WORD SPHERE_FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

namespace si3
{
	SphereData::SphereData()
	{
		construct();
	}
	SphereData::SphereData(LPDIRECT3DDEVICE9 device, const char * path, float smoothness, float radius)
	{
		construct();
		load(device, path, smoothness, radius);
	}

	void SphereData::load(LPDIRECT3DDEVICE9 device, const char * path, float smoothness, float radius)
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


	void SphereData::draw(const D3DXMATRIX & world_mat) const
	{
		// ワールド変換行列設定
		device->SetTransform(D3DTS_WORLD, &world_mat);

		// 頂点フォーマット設定
		device->SetFVF(SPHERE_FVF);


		device->SetTexture(0, texture);

		mesh->DrawSubset(0);

	}


	SphereData::~SphereData()
	{
		release();
	}

	void SphereData::construct()
	{
		device = nullptr;
		mesh = nullptr;
		texture = nullptr;
	}
	void SphereData::release()
	{
		dxsaferelease(mesh);
		dxsaferelease(texture);
	}


}

#endif