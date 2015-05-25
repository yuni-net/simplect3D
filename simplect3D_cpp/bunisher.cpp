#include <bunisher.h>
#include <manager.h>
#include <model.h>
#include <utility.h>
#include <si3_ModelDustConverter.h>

const float alpha_speed = 0.01f;

namespace si3
{
	static const DWORD model_fvf = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;



	bunisher::bunisher(const manager & manageri, const model & model_)
	{
		construct();
		init(manageri, model_);
	}

	bool bunisher::create_buffer(unsigned long buffer_size)
	{
		release();

		// 頂点情報格納バッファを作成
		HRESULT hr = device->CreateVertexBuffer(
			sizeof(top_type)* buffer_size,
			D3DUSAGE_WRITEONLY,
			model_fvf,
			D3DPOOL_MANAGED,
			&listbuff,
			NULL);

		if (FAILED(hr)) return false;

		return true;
	}


	void bunisher::init(const manager & manageri, const model & model_)
	{
		typedef unsigned short ushort;

		device = manageri.get_dxdevice();
		si3m = &manageri;

		const model_data & modeld = model_.model_data();
		unsigned long top_num = modeld.index_num();
		triangle_num = top_num/3;
		bunish_data_list.resize(triangle_num);
		create_buffer(top_num);

		top_type * planebuff = modeld.lock_top_buffer();
		ushort * indexbuff = modeld.lock_index_buffer();

		convert_without_index(planebuff, indexbuff, modeld.index_num());

		modeld.unlock_top_buffer();
		modeld.unlock_index_buffer();

		alpha = 1.0f;
	}



	void bunisher::bunish()
	{
		top_type * top_header;
		listbuff->Lock(0, 0, fw::pointer_cast<void **>(&top_header), 0);
		if (top_header == nullptr)
		{
			return;
		}

		uint top_No = 0;
		auto iter = bunish_data_list.begin();

		while (iter != bunish_data_list.end())
		{
			bunish_data & bunishd = *iter;
			bunishd.bunish(*si3m);

			top_header[top_No + 0].pos = bunishd.compute_local_pos(0);
			top_header[top_No + 1].pos = bunishd.compute_local_pos(1);
			top_header[top_No + 2].pos = bunishd.compute_local_pos(2);

			top_No += 3;
			++iter;
		}

		listbuff->Unlock();

		alpha -= alpha_speed;
#ifdef NDEBUG
		bunish_data_list.sort();	// 降順にソートされる
#endif
	}





	bunisher::bunisher()
	{
		construct();
	}

	bunisher::~bunisher()
	{
		release();
	}

	void bunisher::draw_no_alpha() const
	{
		// Nothing
	}

	void bunisher::draw_alpha() const
	{
		// ワールド変換行列設定
		device->SetTransform(D3DTS_WORLD, matrix().dxpointer());

		// Zバッファを更新しない
		device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		// αテスト有効
		device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

		// 半透明描画有効
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		// ライティング有り
		device->LightEnable(0, TRUE);
		device->SetRenderState(D3DRS_LIGHTING, TRUE);

		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	// カリング無し

		// 頂点フォーマット設定
		device->SetFVF(model_fvf);

		//テクスチャ設定
		device->SetTexture(0, NULL);

		D3DMATERIAL9 material_;
		material_.Diffuse = { 0.28f, 0.88f, 0.88f, alpha };
		material_.Ambient = {0.0f,0.0f,0.0f,0.0f};
		material_.Emissive = { 0.0f, 0.0f, 0.0f, 0.0f };
		material_.Specular = { 1.0f, 1.0f, 1.0f, alpha };
		material_.Power = 1.0f;

		//マテリアル設定
		device->SetMaterial(&material_);

		device->SetStreamSource(0, listbuff, 0, sizeof(top_type));



		device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, triangle_num);



		// Zバッファを更新する
		device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		// 半透明描画無効
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

		// αテスト禁止
		device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}


	void bunisher::construct()
	{
		device = nullptr;
		listbuff = nullptr;
	}

	void bunisher::release()
	{
		saferelease(listbuff);
	}


	void bunisher::convert_without_index(top_type * planebuff, unsigned short * indexbuff, unsigned long index_num)
	{
		top_type * top_header;
		listbuff->Lock(0, 0, fw::pointer_cast<void **>(&top_header), 0);
		if (top_header == nullptr)
		{
			return;
		}

		unsigned long top_num = index_num;
		uint top_No = 0;
		auto iter = bunish_data_list.begin();

		while (iter != bunish_data_list.end())
		{
			bunish_data & bunishd = *iter;

			ModelDustConverter verter;
			verter.init(planebuff, indexbuff, top_No);
			verter.set_bunish_data(bunishd);

			top_header[top_No + 0] = verter.get_top_data(0);
			top_header[top_No + 1] = verter.get_top_data(1);
			top_header[top_No + 2] = verter.get_top_data(2);

			top_No += 3;
			++iter;
		}

		listbuff->Unlock();
	}

#if 0

	void bunisher::set_bunish_vec_list(top_type * top_head, unsigned long top_No)
	{
		if (top_No % 3 != 0)
		{
			return;
		}

		auto & top = top_head[top_No];
		for (unsigned long top_count = 0; top_count < 3; ++top_count)
		{
			unsigned long bunish_No = top_No + top_count;
			if (bunish_No >= top_num)
			{
				return;
			}

			bunish_vec_list[top_No + top_count].x = top.normal.x / 30.0f;
			bunish_vec_list[top_No + top_count].y = top.normal.y / 30.0f;
			bunish_vec_list[top_No + top_count].z = top.normal.z / 30.0f;
		}
	}





#endif





}