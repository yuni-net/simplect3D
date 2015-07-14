#include <model_data.h>
#include <Manager.h>

namespace si3
{
	static const DWORD model_fvf = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	model_data::model_data(const Manager & manageri, const TCHAR * path)
	{
		construct();
		load(manageri, path);
	}
	model_data::model_data(const Manager & manageri, const tstring & path)
	{
		construct();
		load(manageri, path);
	}
	model_data::model_data(const Manager & manageri, const TCHAR * path, const si3::coor3 & center)
	{
		load(manageri, path, center);
	}

#pragma pack(push, 1)

	bool model_data::load_header(FILE * fp)
	{
		struct header
		{
			char magic[3];	// "Pmd"
			float version;
			char model_name[20];
			char comment[256];
		};

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

	bool model_data::create_top_buffer(unsigned long top_num)
	{
		// 頂点情報格納バッファを作成
		HRESULT hr = device->CreateVertexBuffer(
			sizeof(top_type)* top_num,
			D3DUSAGE_WRITEONLY,
			model_fvf,
			D3DPOOL_MANAGED,
			&vertbuff,
			NULL);

		if (FAILED(hr)) return false;

		return true;
	}

	bool model_data::load_top_center(FILE * fp, const si3::coor3 & center)
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
		top_type * top_head = nullptr;
		HRESULT hr = vertbuff->Lock(0, 0, fw::pointer_cast<void **>(&top_head), 0);
		if (FAILED(hr)) return false;

		for (ulong top_No = 0; top_No < top_num; ++top_No)
		{
			struct top_data
			{
				float pos[3];
				float normal[3];
				float uv[2];
				ushort bone_num[2];
				uchar bone_weight;
				uchar edge_flag;
			};

			top_data top_data_;
			fread(&top_data_, sizeof(top_data), 1, fp);

			top_type & top = top_head[top_No];
			top.pos.x = top_data_.pos[0] - center.x;
			top.pos.y = top_data_.pos[1] - center.y;
			top.pos.z = top_data_.pos[2] - center.z;
			top.normal.x = top_data_.normal[0];
			top.normal.y = top_data_.normal[1];
			top.normal.z = top_data_.normal[2];
			top.u = top_data_.uv[0];
			top.v = top_data_.uv[1];
		}

		vertbuff->Unlock();

		return true;
	}

	bool model_data::load_top(FILE * fp)
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
		top_type * top_head = nullptr;
		HRESULT hr = vertbuff->Lock(0, 0, fw::pointer_cast<void **>(&top_head), 0);
		if (FAILED(hr)) return false;

		for (ulong top_No = 0; top_No < top_num; ++top_No)
		{
			struct top_data
			{
				float pos[3];
				float normal[3];
				float uv[2];
				ushort bone_num[2];
				uchar bone_weight;
				uchar edge_flag;
			};

			top_data top_data_;
			fread(&top_data_, sizeof(top_data), 1, fp);

			top_type & top = top_head[top_No];
			top.pos.x = top_data_.pos[0];
			top.pos.y = top_data_.pos[1];
			top.pos.z = top_data_.pos[2];
			top.normal.x = top_data_.normal[0];
			top.normal.y = top_data_.normal[1];
			top.normal.z = top_data_.normal[2];
			top.u = top_data_.uv[0];
			top.v = top_data_.uv[1];
		}

		vertbuff->Unlock();

		return true;
	}

	bool model_data::create_index_buffer(unsigned long index_num)
	{
		// 頂点インデックスバッファ作成
		HRESULT hr = device->CreateIndexBuffer(
			sizeof(unsigned long)* index_num,
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&indexbuff,
			NULL);

		if (FAILED(hr)) return false;

		return true;
	}

	bool model_data::load_index(FILE * fp)
	{
		typedef unsigned long ulong;
		typedef unsigned short ushort;

		fread(&index_num_, sizeof(ulong), 1, fp);

		if (create_index_buffer(index_num_) == false)
		{
			return false;
		}

		// バッファをロックして書き込み開始
		ushort * index_head = nullptr;
		HRESULT hr = indexbuff->Lock(0, 0, fw::pointer_cast<void **>(&index_head), 0);
		if (FAILED(hr)) return false;

		for (ulong index_No = 0; index_No < index_num_; ++index_No)
		{
			ushort index_data;
			fread(&index_data, sizeof(ushort), 1, fp);
			index_head[index_No] = index_data;
		}

		indexbuff->Unlock();

		return true;
	}

	void model_data::load_material(D3DMATERIAL9 & mate, const pmd_mate_data & mate_data)
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

	bool model_data::load_texture(LPDIRECT3DTEXTURE9 & texture, char tex_name[20], const TCHAR * path)
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
			device,
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

	bool model_data::load_attbute(FILE * fp, const TCHAR * path)
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

			pmd_mate_data mate_data;
			fread(&mate_data, sizeof(pmd_mate_data), 1, fp);

			attbute & attbute_ = attbute_list[attbute_No];

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


	bool model_data::load(const Manager & manageri, const TCHAR * path, const si3::coor3 & center)
	{
		release();

		this->device = manageri.get_dxdevice();

		FILE * fp = fopen(path, "rb");

		if (load_header(fp) == false)
		{
			return false;
		}

		if (load_top_center(fp, center) == false)
		{
			return false;
		}
		if (load_index(fp) == false)
		{
			return false;
		}

		if (load_attbute(fp, path) == false)
		{
			return false;
		}

		if (fp)
		{
			fclose(fp);
		}

		return true;
	}
	bool model_data::load(const Manager & manageri, const TCHAR * path)
	{
		release();

		this->device = manageri.get_dxdevice();

		FILE * fp = fopen(path, "rb");

		if (load_header(fp) == false)
		{
			return false;
		}

		if (load_top(fp) == false)
		{
			return false;
		}
		if (load_index(fp) == false)
		{
			return false;
		}

		if (load_attbute(fp, path) == false)
		{
			return false;
		}

		if (fp)
		{
			fclose(fp);
		}

		return true;
	}

#pragma pack(pop)


	bool model_data::load(const Manager & manageri, const tstring & path)
	{
		return load(manageri, path.c_str());
	}



	unsigned long model_data::index_num() const
	{
		return index_num_;
	}

	top_type * model_data::lock_top_buffer() const
	{
		top_type * top_header;
		vertbuff->Lock(0, 0, fw::pointer_cast<void **>(&top_header), 0);
		return top_header;
	}
	void model_data::unlock_top_buffer() const
	{
		vertbuff->Unlock();
	}

	unsigned short * model_data::lock_index_buffer() const
	{
		unsigned short * index_header;
		indexbuff->Lock(0, 0, fw::pointer_cast<void **>(&index_header), 0);
		return index_header;
	}
	void model_data::unlock_index_buffer() const
	{
		indexbuff->Unlock();
	}



	model_data::model_data()
	{
		construct();
	}

	model_data::~model_data()
	{
		release();
	}

	void model_data::draw_no_alpha(const matrix & world_mat, bool is_culling) const
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

			auto & material_ = attbute.material;
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
				device->LightEnable(0, FALSE);
				device->SetRenderState(D3DRS_LIGHTING, FALSE);

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
			device->SetMaterial(&material_);

			//テクスチャ設定
			device->SetTexture(0, texture_);

			device->SetStreamSource(0, vertbuff, 0, sizeof(top_type));
			device->SetIndices(indexbuff);

			uint index_num = use_top_num;
			uint triangle_num = index_num / 3;

			// プリミティブ描画
			device->DrawIndexedPrimitive(
				D3DPT_TRIANGLELIST,
				0,
				0,
				index_num,
				beg_index_now,
				triangle_num);
		}
	}

	void model_data::draw_alpha(const matrix & world_mat) const
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

			auto & material_ = attbute.material;
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
			device->SetMaterial(&material_);

			//テクスチャ設定
			device->SetTexture(0, texture_);

			device->SetStreamSource(0, vertbuff, 0, sizeof(top_type));
			device->SetIndices(indexbuff);

			uint index_num = use_top_num;
			uint triangle_num = index_num / 3;

			// プリミティブ描画
			device->DrawIndexedPrimitive(
				D3DPT_TRIANGLELIST,
				0,
				0,
				index_num,
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


	void model_data::construct()
	{
		device = nullptr;
		vertbuff = nullptr;
		indexbuff = nullptr;
	}

	void model_data::release()
	{
		saferelease(vertbuff);
		saferelease(indexbuff);

		for (uint attbute_No = 0; attbute_No < attbute_list.size(); ++attbute_No)
		{
			saferelease(attbute_list[attbute_No].texture);
		}
	}

#if 0

	bool model_data::load_x(const Manager & manageri, const TCHAR * path, const si3::coor3 & center)
	{
		release();

		this->dxdevice = manageri.get_dxdevice();

		LPD3DXBUFFER	pMtrlBuf;

		HRESULT hr = D3DXLoadMeshFromX(
			path,								// ファイル名
			D3DXMESH_SYSTEMMEM,					// 常に同じ指定
			manageri.get_dxdevice(),			// Direct3Dデバイス
			NULL,								// 常にNULL
			&pMtrlBuf,							// マテリアルバッファ
			NULL,								// 常にNULL
			&material_num,						// マテリアル数
			&mesh);								// メッシュオブジェクト
		if (FAILED(hr)) {
			return false;
		}

		LPD3DXMESH pMeshWk;
		add_normal(mesh, manageri.get_dxdevice(), pMeshWk);

		if (get_table(pMeshWk, pMtrlBuf) == false)
		{
			return false;
		}

		compute_tops_center(mesh, center);

		get_material(path, pMtrlBuf, manageri.get_dxdevice());

		judge_with_alpha();

		return true;
	}

	bool model_data::load_x(const Manager & manageri, const TCHAR * path)
	{
		release();

		this->dxdevice = manageri.get_dxdevice();

		LPD3DXBUFFER	pMtrlBuf;

		HRESULT hr = D3DXLoadMeshFromX(
			path,								// ファイル名
			D3DXMESH_SYSTEMMEM,					// 常に同じ指定
			manageri.get_dxdevice(),			// Direct3Dデバイス
			NULL,								// 常にNULL
			&pMtrlBuf,							// マテリアルバッファ
			NULL,								// 常にNULL
			&material_num,						// マテリアル数
			&mesh);								// メッシュオブジェクト
		if (FAILED(hr)) {
			return false;
		}

		LPD3DXMESH pMeshWk;
		add_normal(mesh, manageri.get_dxdevice(), pMeshWk);

		if (get_table(pMeshWk, pMtrlBuf) == false)
		{
			return false;
		}

		compute_tops(mesh);

		get_material(path, pMtrlBuf, manageri.get_dxdevice());

		judge_with_alpha();

		return true;
	}

	void model_data::add_normal(LPD3DXMESH & mesh, const LPDIRECT3DDEVICE9 & device, LPD3DXMESH & pMeshWk)
	{
		// 法線が無い場合は強制的に追加
		DWORD dwFVF = mesh->GetFVF();
		bool has_xyz_normal_uv;
		if ((dwFVF & (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)) == (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1))
		{
			has_xyz_normal_uv = true;
		}
		if ((dwFVF & D3DFVF_NORMAL) == 0) {
			HRESULT hr = mesh->CloneMeshFVF(mesh->GetOptions(), dwFVF | D3DFVF_NORMAL, device, &pMeshWk);
			if (SUCCEEDED(hr)) {
				D3DXComputeNormals(pMeshWk, NULL);
				mesh->Release();
				mesh = pMeshWk;
			}
		}
	}

	bool model_data::get_table(LPD3DXMESH & pMeshWk, LPD3DXBUFFER & pMtrlBuf)
	{
		// 属性テーブルを生成するための最適化
		HRESULT hr = mesh->Optimize(D3DXMESHOPT_ATTRSORT, NULL, NULL, NULL, NULL, &pMeshWk);
		if (SUCCEEDED(hr)) {
			mesh->Release();
			mesh = pMeshWk;
		}
		else {
			saferelease(pMtrlBuf);
			return false;
		}

		// 属性テーブル取得
		hr = mesh->GetAttributeTable(NULL, &attribute_num);
		if (FAILED(hr)) {
			saferelease(pMtrlBuf);
			saferelease(mesh);
			return false;
		}
		attribute = new D3DXATTRIBUTERANGE[attribute_num];
		hr = mesh->GetAttributeTable(attribute, &attribute_num);

		return true;
	}

	void model_data::compute_tops(LPD3DXMESH & mesh)
	{
		// 頂点バッファ固定
		LPD3DXVECTOR3 pVtx;
		mesh->LockVertexBuffer(0, (LPVOID*)&pVtx);
		// 境界ボックス生成
		D3DXVECTOR3 vMin = *pVtx;
		D3DXVECTOR3 vMax = vMin;
		DWORD dwFaceNum = mesh->GetNumFaces();
		DWORD dwVtx = mesh->GetNumVertices();
		DWORD dwStroke = mesh->GetNumBytesPerVertex();
		LPD3DXVECTOR3 pV = pVtx;
		for (DWORD i = 0; i < dwVtx; i++) {
			//	pV->x *= fScale;
			//	pV->y *= fScale;
			//	pV->z *= fScale;
			if (vMin.x > pV->x) vMin.x = pV->x;
			if (vMin.y > pV->y) vMin.y = pV->y;
			if (vMin.z > pV->z) vMin.z = pV->z;
			if (vMax.x < pV->x) vMax.x = pV->x;
			if (vMax.y < pV->y) vMax.y = pV->y;
			if (vMax.z < pV->z) vMax.z = pV->z;
			pV = (LPD3DXVECTOR3)((LPBYTE)pV + dwStroke);
		}
		vBBox = (vMax - vMin) / 2.0f;
		vCenter = (vMax + vMin) / 2.0f;
		// 境界球生成
		fRadius = 0.0f;
		float fR;
		pV = pVtx;
		for (DWORD i = 0; i < dwVtx; i++) {
			fR = D3DXVec3Length(&(*pV - vCenter));
			if (fRadius < fR)
				fRadius = fR;
			pV = (LPD3DXVECTOR3)((LPBYTE)pV + dwStroke);
		}
		// 頂点バッファ解放
		mesh->UnlockVertexBuffer();
	}
	void model_data::compute_tops_center(LPD3DXMESH & mesh, const coor3 & center)
	{
		// 頂点バッファ固定
		LPD3DXVECTOR3 pVtx;
		mesh->LockVertexBuffer(0, (LPVOID*)&pVtx);
		// 境界ボックス生成
		D3DXVECTOR3 vMin = *pVtx;
		D3DXVECTOR3 vMax = vMin;
		DWORD dwFaceNum = mesh->GetNumFaces();
		DWORD dwVtx = mesh->GetNumVertices();
		DWORD dwStroke = mesh->GetNumBytesPerVertex();
		LPD3DXVECTOR3 pV = pVtx;
		for (DWORD i = 0; i < dwVtx; i++) {
			pV->x -= center.x;
			pV->y -= center.y;
			pV->z -= center.z;
			if (vMin.x > pV->x) vMin.x = pV->x;
			if (vMin.y > pV->y) vMin.y = pV->y;
			if (vMin.z > pV->z) vMin.z = pV->z;
			if (vMax.x < pV->x) vMax.x = pV->x;
			if (vMax.y < pV->y) vMax.y = pV->y;
			if (vMax.z < pV->z) vMax.z = pV->z;
			pV = (LPD3DXVECTOR3)((LPBYTE)pV + dwStroke);
		}
		vBBox = (vMax - vMin) / 2.0f;
		vCenter = (vMax + vMin) / 2.0f;
		// 境界球生成
		fRadius = 0.0f;
		float fR;
		pV = pVtx;
		for (DWORD i = 0; i < dwVtx; i++) {
			fR = D3DXVec3Length(&(*pV - vCenter));
			if (fRadius < fR)
				fRadius = fR;
			pV = (LPD3DXVECTOR3)((LPBYTE)pV + dwStroke);
		}
		// 頂点バッファ解放
		mesh->UnlockVertexBuffer();
	}

	void model_data::get_material(const TCHAR * path, LPD3DXBUFFER	pMtrlBuf, const LPDIRECT3DDEVICE9 & device)
	{
		TCHAR			szDir[_MAX_DIR];
		TCHAR			szSaveDir[_MAX_DIR];

		// カレントディレクトリをXファイルのあるフォルダに移動
		_tsplitpath(path, NULL, szDir, NULL, NULL);
		if (szDir[0]) {	// フォルダ相対指定あり?
			GetCurrentDirectory(_countof(szSaveDir), szSaveDir);
			SetCurrentDirectory(szDir);
		}

		// マテリアルの抽出
		D3DXMATERIAL* pMtrl = (D3DXMATERIAL*)pMtrlBuf->GetBufferPointer();
		material = new D3DMATERIAL9[material_num];
		ppTexture = new LPDIRECT3DTEXTURE9[material_num];
		for (DWORD i = 0; i < material_num; i++) {
			material[i] = pMtrl[i].MatD3D;
			// Xファイルには環境色が無い、拡散色で上書きしておく.
			material[i].Ambient = material[i].Diffuse;
			ppTexture[i] = NULL;
			if (pMtrl[i].pTextureFilename && pMtrl[i].pTextureFilename[0]) {
				HRESULT hr = D3DXCreateTextureFromFileA(device, pMtrl[i].pTextureFilename, &ppTexture[i]);
				if (FAILED(hr)) {
					break;
				}
			}
		}

		// カレントディレクトリを元に戻す
		if (szDir[0]) {	// フォルダ相対指定あり?
			SetCurrentDirectory(szSaveDir);
		}

		pMtrlBuf->Release();
	}

	void model_data::judge_with_alpha()
	{
		for (DWORD i = 0; i < attribute_num; i++) {
			DWORD id = attribute[i].AttribId;
			if (material[id].Diffuse.a < 1.0f) {
				with_alpha = true;
				break;
			}
		}
	}




	void model_data::compute_world_mat(D3DXMATRIX & mat, const model_coor & coor) const
	{
		D3DXMATRIX matRot, matMove;
		D3DXMatrixRotationYawPitchRoll(&matRot, coor.radian_y, coor.radian_x, coor.radian_z);
		D3DXMatrixTranslation(&matMove, coor.x, coor.y, coor.z);
		D3DXMatrixMultiply(&mat, &matRot, &matMove);

	}
#endif

}