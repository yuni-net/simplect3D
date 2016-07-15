#include <si3_Triangle.h>
#include <si3_popular.h>
#include <si3_Manager.h>
#include <si3_Matrix.h>

static const WORD fvf = D3DFVF_XYZ | D3DFVF_NORMAL;

struct Top
{
	D3DVECTOR   pos;
	D3DVECTOR   normal;
};

namespace si3
{
	Triangle::Triangle(const Coor3 & a, const Coor3 & b, const Coor3 & c)
	{
		LPDIRECT3DDEVICE9 device = si3::Manager::get_dxdevice();
		HRESULT	hr;

		// 頂点情報格納バッファを作成
		hr = device->CreateVertexBuffer(
			sizeof(Top)* 3,
			D3DUSAGE_WRITEONLY,
			fvf,
			D3DPOOL_MANAGED,
			&vertbuff,
			NULL);

		if (FAILED(hr)) return;

		// バッファをロックをして書き込みを開始する
		Top * vert_arr = nullptr;
		hr = vertbuff->Lock(0, 0, fw::pointer_cast<void **>(&vert_arr), 0);
		if (FAILED(hr)) return;

		vert_arr[0].pos = a.dxvec3();
		vert_arr[0].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vert_arr[1].pos = b.dxvec3();
		vert_arr[1].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vert_arr[2].pos = c.dxvec3();
		vert_arr[2].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// バッファをアンロックして書き込みを終了する
		vertbuff->Unlock();
	}

	void Triangle::draw_no_alpha() const
	{
		LPDIRECT3DDEVICE9 device = si3::Manager::get_dxdevice();

		// Zバッファを更新する
		device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		// αテスト禁止
		device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

		// 半透明表示禁止(αブレンディング無効化)
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

		// マテリアルとテクスチャのrgba情報を掛け合わせる設定
		device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTA_DIFFUSE);
		device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);

		// ワールド変換行列設定
		D3DXMATRIX world_mat = Matrix().parallel(x,y,z).dxmat();
		device->SetTransform(D3DTS_WORLD, &world_mat);

		// 頂点フォーマット設定
		device->SetFVF(fvf);

		//マテリアル設定
		D3DMATERIAL9 material;
		material.Diffuse.a = 1.0f;
		material.Diffuse.r = 1.0f;
		material.Diffuse.g = 0.68f;
		material.Diffuse.b = 0.68f;

		material.Ambient.a = 1.0f;
		material.Ambient.r = 1.0f;
		material.Ambient.g = 0.68f;
		material.Ambient.b = 0.68f;

		material.Specular.a = 0.0f;
		material.Specular.r = 0.0f;
		material.Specular.g = 0.0f;
		material.Specular.b = 0.0f;

		material.Emissive.a = 0.0f;
		material.Emissive.r = 0.0f;
		material.Emissive.g = 0.0f;
		material.Emissive.b = 0.0f;

		material.Power = 1.0f;
		device->SetMaterial(&material);

		device->SetStreamSource(0, vertbuff, 0, sizeof(Top));


		device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		//device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		//device->SetRenderState(D3DRS_FILLMODE, D3DFILL_POINT);

		// プリミティブ描画
		device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 1);
	}

	void Triangle::draw_alpha() const
	{
		// Nothing
	}







}