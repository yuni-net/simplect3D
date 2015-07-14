#include <ModelData.h>
#include <Manager.h>
#include <MotionData.h>
#include <Top_pmd.h>
#include <cstdint>
#include <BoneMap.h>

namespace si3
{
	static const DWORD model_fvf = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	ModelData::ModelData(const Manager & manageri, const TCHAR * path)
	{
		construct();
		load(manageri, path);
	}
	ModelData::ModelData(const Manager & manageri, const tstring & path)
	{
		construct();
		load(manageri, path);
	}
	ModelData::ModelData(const Manager & manageri, const TCHAR * path, const si3::Coor3 & center)
	{
		load(manageri, path, center);
	}


	bool ModelData::load_header(FILE * fp)
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

	bool ModelData::load_bone(FILE * fp, BoneMap & bone_map)
	{
#pragma pack(push, 1)
		struct BoneData
		{
			char bone_name[20];
			int16_t parent_bone_index;
			int16_t tail_pos_bone_index;
			int8_t bone_type;
			int16_t ik_parent_bone_index;
			float bone_head_pos[3];
		};
#pragma pack(pop)

		// seek top datas *******************************
		unsigned long vert_num;
		fread(&vert_num, sizeof(vert_num), 1, fp);
		fseek(fp, sizeof(top_data)*vert_num, SEEK_CUR);
		// **********************************************

		// seek index datas *****************************
		unsigned long index_num;
		fread(&index_num, sizeof(index_num), 1, fp);
		fseek(fp, sizeof(unsigned short)*index_num, SEEK_CUR);
		// **********************************************

		// seek material datas **************************
		unsigned long material_num;
		fread(&material_num, sizeof(material_num), 1, fp);
		fseek(fp, sizeof(pmd_mate_data)*material_num, SEEK_CUR);
		// **********************************************

		unsigned short bone_num;
		fread(&bone_num, sizeof(bone_num), 1, fp);
		bone_tree.setsize(bone_num);
		bone_num_ = bone_num;

		for (unsigned short bone_No = 0; bone_No < bone_num; ++bone_No)
		{
			BoneData bone_data;
			fread(&bone_data, sizeof(bone_data), 1, fp);

			const std::string bone_name = get_text(bone_data.bone_name, 20);
			bone_map.register_name(bone_name);

			BoneBranch & me = bone_tree[bone_No];
			me.init(bone_No, bone_data.bone_head_pos, bone_name);

			const uint16_t parent_No = bone_data.parent_bone_index;
			// debug
			if (parent_No == 38)
			{
				int a = 0;
			}
			const bool there_is_no_parent = parent_No == 0xffff;
			if (there_is_no_parent)
			{
				continue;
			}

			BoneBranch & parent = bone_tree[parent_No];
			parent.add_child(me);
		}

		long reverse_byte =
			sizeof(vert_num) + sizeof(top_data)*vert_num +
			sizeof(index_num) + sizeof(unsigned short)*index_num +
			sizeof(material_num) + sizeof(pmd_mate_data)*material_num +
			sizeof(bone_num) + sizeof(BoneData)*bone_num;

		fseek(fp, -reverse_byte, SEEK_CUR);

		return true;
	}


	bool ModelData::create_top_buffer(unsigned long top_num)
	{
		// 頂点情報格納バッファを作成
		HRESULT hr = device->CreateVertexBuffer(
			sizeof(DxTop)* top_num,
			D3DUSAGE_WRITEONLY,
			model_fvf,
			D3DPOOL_MANAGED,
			&vertbuff,
			NULL);

		if (FAILED(hr)) return false;

		return true;
	}

	bool ModelData::load_top_center(FILE * fp, const si3::Coor3 & center)
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
		HRESULT hr = vertbuff->Lock(0, 0, fw::pointer_cast<void **>(&top_head), 0);
		if (FAILED(hr)) return false;

		for (ulong top_No = 0; top_No < top_num; ++top_No)
		{
			top_data top_data_;
			fread(&top_data_, sizeof(top_data), 1, fp);

			DxTop & top = top_head[top_No];
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

	bool ModelData::load_top(FILE * fp)
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
		HRESULT hr = vertbuff->Lock(0, 0, fw::pointer_cast<void **>(&top_head), 0);
		if (FAILED(hr)) return false;

		for (ulong top_No = 0; top_No < top_num; ++top_No)
		{

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

		vertbuff->Unlock();

		return true;
	}

	bool ModelData::load_top(FILE * fp, MotionData & motion_data)
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
		HRESULT hr = vertbuff->Lock(0, 0, fw::pointer_cast<void **>(&top_head), 0);
		if (FAILED(hr)) return false;

		for (ulong top_No = 0; top_No < top_num; ++top_No)
		{

			Top_pmd top_data_;
			fread(&top_data_, sizeof(Top_pmd), 1, fp);

			// todo weightが全く考慮されていない
			const int bone_index = top_data_.bone_num[0];
			bone_tree[bone_index].add_top(top_No, top_data_);

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

		vertbuff->Unlock();

		return true;
	}

	bool ModelData::create_index_buffer(unsigned long index_num)
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

	bool ModelData::load_index(FILE * fp)
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

	void ModelData::load_material(D3DMATERIAL9 & mate, const pmd_mate_data & mate_data)
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

	bool ModelData::load_texture(LPDIRECT3DTEXTURE9 & texture, char tex_name[20], const TCHAR * path)
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

	bool ModelData::load_attbute(FILE * fp, const TCHAR * path)
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


	bool ModelData::load(const Manager & manageri, const TCHAR * path, const si3::Coor3 & center)
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
	bool ModelData::load(const Manager & manageri, const TCHAR * path)
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
	bool ModelData::load(const Manager & manageri, const TCHAR * path, MotionData & motion_data, BoneMap & bone_map)
	{
		release();

		this->device = manageri.get_dxdevice();

		FILE * fp = fopen(path, "rb");

		if (load_header(fp) == false)
		{
			return false;
		}

		if (load_bone(fp, bone_map) == false)
		{
			return false;
		}

		if (load_top(fp, motion_data) == false)
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




	bool ModelData::load(const Manager & manageri, const tstring & path)
	{
		return load(manageri, path.c_str());
	}



	unsigned long ModelData::index_num() const
	{
		return index_num_;
	}

	DxTop * ModelData::lock_top_buffer() const
	{
		DxTop * top_header;
		vertbuff->Lock(0, 0, fw::pointer_cast<void **>(&top_header), 0);
		return top_header;
	}
	void ModelData::unlock_top_buffer() const
	{
		vertbuff->Unlock();
	}

	unsigned short * ModelData::lock_index_buffer() const
	{
		unsigned short * index_header;
		indexbuff->Lock(0, 0, fw::pointer_cast<void **>(&index_header), 0);
		return index_header;
	}
	void ModelData::unlock_index_buffer() const
	{
		indexbuff->Unlock();
	}

	int ModelData::bone_num() const
	{
		return bone_num_;
	}


	void ModelData::animation(MotionData & motion_data)
	{
		bool motion_is_not_needed = motion_data.animation();
		if (motion_is_not_needed)
		{
			return;
		}

		auto top_buffer = lock_top_buffer();

		bool todo_renew = false;
		auto & root = bone_tree[0];
		root.renew_tops(top_buffer, motion_data, matrix(), matrix(), todo_renew);

		unlock_top_buffer();
	}


	ModelData::ModelData()
	{
		construct();
	}

	ModelData::~ModelData()
	{
		release();
	}

	void ModelData::draw_no_alpha() const
	{

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

			device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

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

			device->SetStreamSource(0, vertbuff, 0, sizeof(DxTop));
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

	void ModelData::draw_alpha() const
	{

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

			device->SetStreamSource(0, vertbuff, 0, sizeof(DxTop));
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


	void ModelData::construct()
	{
		device = nullptr;
		vertbuff = nullptr;
		indexbuff = nullptr;
	}

	void ModelData::release()
	{
		dxsaferelease(vertbuff);
		dxsaferelease(indexbuff);

		for (uint attbute_No = 0; attbute_No < attbute_list.size(); ++attbute_No)
		{
			dxsaferelease(attbute_list[attbute_No].texture);
		}
	}

}