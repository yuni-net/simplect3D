#include <si3_MorfData.h>
#include <si3_Manager.h>

#pragma warning(push)
#pragma warning(disable:4996)

namespace si3
{
	static const DWORD model_fvf = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	MorfData::MorfData(const tstring & before, const tstring & after)
	{
		this->device = si3::Manager::get_dxdevice();
		this->before.load(before);
		this->after.load(after);
		buff_bytes = this->before.buff_bytes;
		if (buff_bytes < this->after.buff_bytes)
		{
			buff_bytes = this->after.buff_bytes;
		}
		HRESULT hr = si3::Manager::get_dxdevice()->CreateVertexBuffer(
			sizeof(DxTop)* buff_bytes,
			D3DUSAGE_WRITEONLY,
			model_fvf,
			D3DPOOL_MANAGED,
			&vertbuff,
			NULL);

		update(1.0f);
	}

	void MorfData::update(const float percent)
	{
		// update vertbuff
		DxTop * before_buff;
		before.vertbuff->Lock(0, 0, fw::pointer_cast<void **>(&before_buff), 0);
		DxTop * after_buff;
		after.vertbuff->Lock(0, 0, fw::pointer_cast<void **>(&after_buff), 0);
		DxTop * now_buff;
		vertbuff->Lock(0, 0, fw::pointer_cast<void **>(&now_buff), 0);

		for (unsigned int index = 0; index < buff_bytes; ++index)
		{
			const DxTop & before_top = get_top(before_buff, before, index);
			const DxTop & after_top = get_top(after_buff, after, index);
			multiple(before_top, after_top, percent, now_buff[index]);
		}

		vertbuff->Unlock();
		after.vertbuff->Unlock();
		before.vertbuff->Unlock();

		// update now_attbute
		// update blight_percent
		if (percent > 0.5f)
		{
			now_attbute = &before.attbute_list;
			blight_percent = (percent - 0.5f) / 0.5f;
#if 0
			if (percent > 0.75f)
			{
				blight_percent = (percent - 0.75f) / 0.25f;
			}
			else
			{
				blight_percent = 1.0f - (percent - 0.5f) / 0.25f;
			}
#endif
		}
		else
		{
			now_attbute = &after.attbute_list;
			blight_percent = 1.0f - percent / 0.5f;
#if 0
			if (percent > 0.25f)
			{
				blight_percent = (percent - 0.25f) / 0.25f;
			}
			else
			{
				blight_percent = 1.0f - percent / 0.25f;
			}
#endif
		}
	}

	void MorfData::draw_no_alpha(const Matrix & world_mat, bool is_culling) const
	{
		// ワールド変換行列設定
		device->SetTransform(D3DTS_WORLD, world_mat.dxpointer());

		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

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

		uint beg_index = 0;

		auto & attbute_list = *now_attbute;

		// サブセット毎の描画
		for (uint attbute_No = 0; attbute_No < attbute_list.size(); ++attbute_No)
		{
			const uint beg_index_now = beg_index;

			auto & attbute = attbute_list[attbute_No];

			uint use_top_num = attbute.use_top_num;
			if (use_top_num == 0)
			{
				continue;
			}

			beg_index += use_top_num;

			auto material_ = attbute.material;
			auto & texture_ = attbute.texture;

			if (is_culling)
			{
				device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			}
			else
			{
				device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			}

			// ライティング有り
			device->LightEnable(0, TRUE);
			device->SetRenderState(D3DRS_LIGHTING, TRUE);

			float alpha = material_.Diffuse.a;

			if (abs(alpha - 0.99f) < 0.00001f)
			{
				// カリングOFF
				device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
				alpha = 1.0f;
			}
			if (abs(alpha - 0.98f) < 0.00001f)
			{
				// ライティング無し
				if (blight_percent > 0.5f)
				{
					device->LightEnable(0, FALSE);
					device->SetRenderState(D3DRS_LIGHTING, FALSE);
				}

				alpha = 1.0f;
			}

			// 半透明のサブセットは
			// ここでは描かない
			if (alpha < 1.0f)
			{
				continue;
			}

			// 頂点フォーマット設定
			device->SetFVF(model_fvf);

			//マテリアル設定
			material_.Diffuse.r *= blight_percent;
			material_.Diffuse.g *= blight_percent;
			material_.Diffuse.b *= blight_percent;
			material_.Ambient.r *= blight_percent;
			material_.Ambient.g *= blight_percent;
			material_.Ambient.b *= blight_percent;
			device->SetMaterial(&material_);

			//テクスチャ設定
			device->SetTexture(0, texture_);

			device->SetStreamSource(0, vertbuff, 0, sizeof(DxTop));

			uint index_num = use_top_num;
			uint triangle_num = index_num / 3;

			// プリミティブ描画
			device->DrawPrimitive(
				D3DPT_TRIANGLELIST,
				beg_index_now,
				triangle_num);
		}
	}

	void MorfData::draw_alpha(const Matrix & world_mat) const
	{
		// ワールド変換行列設定
		device->SetTransform(D3DTS_WORLD, world_mat.dxpointer());

		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		// αテスト有効
		device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

		// 半透明描画有効
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		// マテリアルとテクスチャのrgba情報を掛け合わせる設定
		device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
		device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);

		// Zバッファを更新しない
		device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		uint beg_index = 0;

		auto & attbute_list = *now_attbute;

		// サブセット毎の描画
		for (uint attbute_No = 0; attbute_No < attbute_list.size(); ++attbute_No)
		{
			const uint beg_index_now = beg_index;

			auto & attbute = attbute_list[attbute_No];

			uint use_top_num = attbute.use_top_num;
			if (use_top_num == 0)
			{
				continue;
			}

			beg_index += use_top_num;

			auto material_ = attbute.material;
			auto & texture_ = attbute.texture;

			float alpha = material_.Diffuse.a;
			if (abs(alpha - 0.99f) < 0.00001f)
			{
				// 0.99はカリングOFF設定の完全不透明ポリゴンだからここでは描画しない
				continue;
			}
			if (abs(alpha - 0.98f) < 0.00001f)
			{
				// 0.98はライトOFF設定の完全不透明ポリゴンだからここでは描画しない
				continue;
			}

			// 完全不透明のサブセットは
			// ここでは描かない
			if (alpha >= 1.0f)
			{
				continue;
			}

			if (alpha < 0.000000001f)
			{
				continue;
			}

			// ライティング有り
			device->LightEnable(0, TRUE);
			device->SetRenderState(D3DRS_LIGHTING, TRUE);

			// 頂点フォーマット設定
			device->SetFVF(model_fvf);

			//マテリアル設定
			material_.Diffuse.r *= blight_percent;
			material_.Diffuse.g *= blight_percent;
			material_.Diffuse.b *= blight_percent;
			material_.Ambient.r *= blight_percent;
			material_.Ambient.g *= blight_percent;
			material_.Ambient.b *= blight_percent;
			device->SetMaterial(&material_);

			//テクスチャ設定
			device->SetTexture(0, texture_);

			device->SetStreamSource(0, vertbuff, 0, sizeof(DxTop));

			uint index_num = use_top_num;
			uint triangle_num = index_num / 3;

			// プリミティブ描画
			device->DrawPrimitive(
				D3DPT_TRIANGLELIST,
				beg_index_now,
				triangle_num);
		}

		// Zバッファを更新する
		device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		// 半透明描画無効
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

		// αテスト禁止
		device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}



	MorfData::~MorfData()
	{
		dxsaferelease(vertbuff);
		release(before);
		release(after);
	}




	bool MorfData::MyModelData::create_top_buffer(unsigned long top_num)
	{
		// 頂点情報格納バッファを作成
		HRESULT hr = si3::Manager::get_dxdevice()->CreateVertexBuffer(
			sizeof(DxTop)* top_num,
			D3DUSAGE_WRITEONLY,
			model_fvf,
			D3DPOOL_MANAGED,
			&base_tops,
			NULL);

		if (FAILED(hr)) return false;

		return true;
	}


	void MorfData::MyModelData::load(const tstring & path)
	{
		FILE * fp = fopen(path.c_str(), "rb");

		if (load_header(fp) == false)
		{
			return;
		}

		if (load_top(fp) == false)
		{
			return;
		}
		if (load_index(fp) == false)
		{
			return;
		}
		if (decompress() == false)
		{
			return;
		}

		if (load_attbute(fp, path.c_str()) == false)
		{
			return;
		}

		dxsaferelease(base_tops);
		dxsaferelease(base_indexes);

		if (fp)
		{
			fclose(fp);
		}
	}

	bool MorfData::MyModelData::load_header(FILE * fp)
	{
#pragma pack(push, 1)
		struct header
		{
			char magic[3];	// "Pmd"
			float version;
			char model_name[20];
			char comment[256];
		};
#pragma pack(pop)

		header header_;

		fread(&header_, sizeof(header), 1, fp);

		if (header_.magic[0] != 'P' ||
			header_.magic[1] != 'm' ||
			header_.magic[2] != 'd')
		{
			return false;
		}

		return true;
	}
	bool MorfData::MyModelData::load_top(FILE * fp)
	{
		typedef unsigned long ulong;
		typedef unsigned short ushort;

		ulong top_num;
		fread(&top_num, sizeof(ulong), 1, fp);

		if (create_top_buffer(top_num) == false)
		{
			return false;
		}

		// バッファをロックをして書き込みを開始する
		DxTop * top_head = nullptr;
		HRESULT hr = base_tops->Lock(0, 0, fw::pointer_cast<void **>(&top_head), 0);
		if (FAILED(hr)) return false;

		for (ulong top_No = 0; top_No < top_num; ++top_No)
		{
#pragma pack(push, 1)
			struct top_data
			{
				float pos[3];
				float normal[3];
				float uv[2];
				ushort bone_num[2];
				uchar bone_weight;
				uchar edge_flag;
			};
#pragma pack(pop)

			top_data top_data_;
			fread(&top_data_, sizeof(top_data), 1, fp);

			DxTop & top = top_head[top_No];
			top.pos.x = top_data_.pos[0];
			top.pos.y = top_data_.pos[1];
			top.pos.z = top_data_.pos[2];
			top.normal.x = top_data_.normal[0];
			top.normal.y = top_data_.normal[1];
			top.normal.z = top_data_.normal[2];
			top.u = top_data_.uv[0];
			top.v = top_data_.uv[1];
		}

		base_tops->Unlock();

		return true;
	}

	bool MorfData::MyModelData::create_index_buffer(unsigned long index_num)
	{
		// 頂点インデックスバッファ作成
		HRESULT hr = si3::Manager::get_dxdevice()->CreateIndexBuffer(
			sizeof(unsigned long)* index_num,
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&base_indexes,
			NULL);

		if (FAILED(hr)) return false;

		return true;
	}

	bool MorfData::MyModelData::load_index(FILE * fp)
	{
		typedef unsigned long ulong;
		typedef unsigned short ushort;

		fread(&buff_bytes, sizeof(ulong), 1, fp);

		if (create_index_buffer(buff_bytes) == false)
		{
			return false;
		}

		// バッファをロックして書き込み開始
		ushort * index_head = nullptr;
		HRESULT hr = base_indexes->Lock(0, 0, fw::pointer_cast<void **>(&index_head), 0);
		if (FAILED(hr)) return false;

		for (ulong index_No = 0; index_No < buff_bytes; ++index_No)
		{
			ushort index_data;
			fread(&index_data, sizeof(ushort), 1, fp);
			index_head[index_No] = index_data;
		}

		base_indexes->Unlock();

		return true;
	}

	bool MorfData::MyModelData::create_answer_buff(unsigned long buff_bytes)
	{
		HRESULT hr = si3::Manager::get_dxdevice()->CreateVertexBuffer(
			sizeof(DxTop)* buff_bytes,
			D3DUSAGE_WRITEONLY,
			model_fvf,
			D3DPOOL_MANAGED,
			&vertbuff,
			NULL);

		if (FAILED(hr)) return false;

		return true;
	}


	bool MorfData::MyModelData::decompress()
	{
		create_answer_buff(buff_bytes);
		DxTop * answer = nullptr;
		HRESULT hr = vertbuff->Lock(0, 0, fw::pointer_cast<void **>(&answer), 0);
		DxTop * top_buffer = nullptr;
		hr = base_tops->Lock(0, 0, fw::pointer_cast<void **>(&top_buffer), 0);
		unsigned short * index_head = nullptr;
		hr = base_indexes->Lock(0, 0, fw::pointer_cast<void **>(&index_head), 0);

		for (unsigned long index = 0; index < buff_bytes; ++index)
		{
			answer[index] = top_buffer[index_head[index]];
		}

		base_indexes->Unlock();
		base_tops->Unlock();
		vertbuff->Unlock();
		return true;
	}

	bool MorfData::MyModelData::load_attbute(FILE * fp, const char * path)
	{
		typedef unsigned long ulong;
		typedef unsigned short ushort;

		ulong material_num;
		fread(&material_num, sizeof(ulong), 1, fp);

		attbute_list.setsize(material_num);
		for (uint attbute_No = 0; attbute_No < attbute_list.size(); ++attbute_No)
		{
			if (attbute_No == 27)
			{
				attbute_No = 27;
			}

			if (attbute_No == 29)
			{
				attbute_No = 29;
			}

			Pmd_mate_data mate_data;
			fread(&mate_data, sizeof(Pmd_mate_data), 1, fp);

			Attbute & attbute_ = attbute_list[attbute_No];

			auto & mate = attbute_.material;
			load_material(mate, mate_data);

			attbute_.use_top_num = mate_data.use_top_num;

			if (load_texture(attbute_.texture, mate_data.texture_name, path) == false)
			{
				return false;
			}
		}

		return true;
	}

	void MorfData::MyModelData::load_material(D3DMATERIAL9 & mate, const Pmd_mate_data & mate_data)
	{
		mate.Diffuse.r = mate_data.diffuse[0];
		mate.Diffuse.g = mate_data.diffuse[1];
		mate.Diffuse.b = mate_data.diffuse[2];
		mate.Diffuse.a = mate_data.alpha;
		mate.Power = mate_data.spec_power;
		mate.Specular.r = mate_data.specular[0];
		mate.Specular.g = mate_data.specular[1];
		mate.Specular.b = mate_data.specular[2];
		mate.Ambient.r = mate_data.ambient[0];
		mate.Ambient.g = mate_data.ambient[1];
		mate.Ambient.b = mate_data.ambient[2];
		mate.Emissive.r = 0.0f;
		mate.Emissive.g = 0.0f;
		mate.Emissive.b = 0.0f;
		mate.Emissive.a = 0.0f;
	}

	bool MorfData::MyModelData::load_texture(LPDIRECT3DTEXTURE9 & texture, char tex_name[20], const TCHAR * path)
	{
		if (tex_name[0] == '\0')
		{
			// テクスチャは無いのでテクスチャ読み込み処理を飛ばす
			texture = nullptr;
			return true;
		}

		// mate_data.texture_nameはヌル文字で終わっているとは限らないから工夫する
		char texture_name[21];
		memcpy(texture_name, tex_name, 20);
		texture_name[20] = '\0';

		// スフィアを無視する
		std::string texture_name_str = texture_name;
		uint index_ast = texture_name_str.find_last_of('*');
		if (index_ast != std::string::npos)
		{
			texture_name[index_ast] = '\0';	// *の部分をヌル文字で埋めて、*以降を無効にする
		}

		std::string tex_path;
		std::string basic_path = path;
		uint index_slash = basic_path.find_last_of("/");
		if (index_slash != std::string::npos)
		{
			tex_path = basic_path.substr(0, index_slash + 1);
		}

		tex_path.append(texture_name);

		// テクスチャ作成
		HRESULT hr = D3DXCreateTextureFromFileEx(
			si3::Manager::get_dxdevice(),
			tex_path.c_str(),
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

		if (FAILED(hr)) return false;

		return true;
	}




	const DxTop & MorfData::get_top(const DxTop * buffer, MyModelData & modeld, const unsigned int index)
	{
		unsigned int myindex = index;
		if (myindex >= modeld.buff_bytes)
		{
			myindex = modeld.buff_bytes - 1;
		}
		return buffer[myindex];
	}

	void MorfData::multiple(const DxTop & one, const DxTop & ano, const float percent, DxTop & answer)
	{
		const float inpercent = 1.0f - percent;

		answer.pos.x = one.pos.x * percent + ano.pos.x*inpercent;
		answer.pos.y = one.pos.y * percent + ano.pos.y*inpercent;
		answer.pos.z = one.pos.z * percent + ano.pos.z*inpercent;

		answer.normal.x = one.normal.x * percent + ano.normal.x*inpercent;
		answer.normal.y = one.normal.y * percent + ano.normal.y*inpercent;
		answer.normal.z = one.normal.z * percent + ano.normal.z*inpercent;

		if (percent > 0.5f)
		{
			answer.u = one.u;
			answer.v = one.v;
		}
		else
		{
			answer.u = ano.u;
			answer.v = ano.v;
		}
	}

	void MorfData::release(MyModelData & modeld)
	{
		dxsaferelease(modeld.vertbuff);

		for (uint attbute_No = 0; attbute_No < modeld.attbute_list.size(); ++attbute_No)
		{
			dxsaferelease(modeld.attbute_list[attbute_No].texture);
		}
	}
}

#pragma warning(pop)