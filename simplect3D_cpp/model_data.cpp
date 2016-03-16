#include <si3_ModelData.h>
#include <si3_Manager.h>

#pragma warning(push)
#pragma warning(disable:4996)

namespace si3
{
	static const DWORD model_fvf = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	ModelData::ModelData(const TCHAR * path)
	{
		construct();
		load(path);
	}
	ModelData::ModelData(const tstring & path)
	{
		construct();
		load(path);
	}
	ModelData::ModelData(const TCHAR * path, const si3::Coor3 & center)
	{
		load(path, center);
	}

#pragma pack(push, 1)

	bool ModelData::load_header(FILE * fp)
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

	bool ModelData::create_top_buffer(unsigned long top_num)
	{
		// ���_���i�[�o�b�t�@���쐬
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

		fread(&top_num, sizeof(ulong), 1, fp);

		if (create_top_buffer(top_num) == false)
		{
			return false;
		}

		// �o�b�t�@�����b�N�����ď������݂��J�n����
		DxTop * top_head = nullptr;
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

		fread(&top_num, sizeof(ulong), 1, fp);

		if (create_top_buffer(top_num) == false)
		{
			return false;
		}

		// �o�b�t�@�����b�N�����ď������݂��J�n����
		DxTop * top_head = nullptr;
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
		// ���_�C���f�b�N�X�o�b�t�@�쐬
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

		// �o�b�t�@�����b�N���ď������݊J�n
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

	void ModelData::load_material(D3DMATERIAL9 & mate, const Pmd_mate_data & mate_data)
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
			// �e�N�X�`���͖����̂Ńe�N�X�`���ǂݍ��ݏ������΂�
			texture = nullptr;
			return true;
		}

		// mate_data.texture_name�̓k�������ŏI����Ă���Ƃ͌���Ȃ�����H�v����
		char texture_name[21];
		memcpy(texture_name, tex_name, 20);
		texture_name[20] = '\0';

		// �X�t�B�A�𖳎�����
		std::string texture_name_str = texture_name;
		uint index_ast = texture_name_str.find_last_of('*');
		if (index_ast != std::string::npos)
		{
			texture_name[index_ast] = '\0';	// *�̕������k�������Ŗ��߂āA*�ȍ~�𖳌��ɂ���
		}

		std::string tex_path;
		std::string basic_path = path;
		uint index_slash = basic_path.find_last_of("/");
		if (index_slash != std::string::npos)
		{
			tex_path = basic_path.substr(0, index_slash + 1);
		}

		tex_path.append(texture_name);

		// �e�N�X�`���쐬
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


	bool ModelData::load(const TCHAR * path, const si3::Coor3 & center)
	{
		release();

		this->device = si3::Manager::get_dxdevice();

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
	bool ModelData::load(const TCHAR * path)
	{
		release();

		this->device = si3::Manager::get_dxdevice();

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


	bool ModelData::load(const tstring & path)
	{
		return load(path.c_str());
	}


	void ModelData::scale(const float value)
	{
		scale(value, value, value);
	}

	void ModelData::scale(const float xval, const float yval, const float zval)
	{
		DxTop * tops = lock_top_buffer();
		for (unsigned long top_No = 0; top_No < top_num; ++top_No)
		{
			DxTop & top = tops[top_No];
			top.pos.x *= xval;
			top.pos.y *= yval;
			top.pos.z *= zval;
		}
		unlock_top_buffer();
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



	ModelData::ModelData()
	{
		construct();
	}

	ModelData::~ModelData()
	{
		release();
	}

	void ModelData::draw_no_alpha(const Matrix & world_mat, bool is_culling) const
	{
		// ���[���h�ϊ��s��ݒ�
		device->SetTransform(D3DTS_WORLD, world_mat.dxpointer());

		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		// Z�o�b�t�@���X�V����
		device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		// ���e�X�g�֎~
		device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

		// �������\���֎~(���u�����f�B���O������)
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

		// �}�e���A���ƃe�N�X�`����rgba�����|�����킹��ݒ�
		device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
		device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);

		uint beg_index = 0;


		// �T�u�Z�b�g���̕`��
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

			// ���C�e�B���O�L��
			device->LightEnable(0, TRUE);
			device->SetRenderState(D3DRS_LIGHTING, TRUE);

			float alpha = material_.Diffuse.a;

			if (abs(alpha - 0.99f) < 0.00001f)
			{
				// �J�����OOFF
				device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
				alpha = 1.0f;
			}
			if (abs(alpha - 0.98f) < 0.00001f)
			{
				// ���C�e�B���O����
				device->LightEnable(0, FALSE);
				device->SetRenderState(D3DRS_LIGHTING, FALSE);

				alpha = 1.0f;
			}

			// �������̃T�u�Z�b�g��
			// �����ł͕`���Ȃ�
			if (alpha < 1.0f)
			{
				continue;
			}

			// ���_�t�H�[�}�b�g�ݒ�
			device->SetFVF(model_fvf);

			//�}�e���A���ݒ�
			device->SetMaterial(&material_);

			//�e�N�X�`���ݒ�
			device->SetTexture(0, texture_);

			device->SetStreamSource(0, vertbuff, 0, sizeof(DxTop));
			device->SetIndices(indexbuff);

			uint index_num = use_top_num;
			uint triangle_num = index_num / 3;

			// �v���~�e�B�u�`��
			device->DrawIndexedPrimitive(
				D3DPT_TRIANGLELIST,
				0,
				0,
				index_num,
				beg_index_now,
				triangle_num);
		}
	}

	void ModelData::draw_alpha(const Matrix & world_mat) const
	{
		// ���[���h�ϊ��s��ݒ�
		device->SetTransform(D3DTS_WORLD, world_mat.dxpointer());

		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		// ���e�X�g�L��
		device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

		// �������`��L��
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		// �}�e���A���ƃe�N�X�`����rgba�����|�����킹��ݒ�
		device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
		device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);

		// Z�o�b�t�@���X�V���Ȃ�
		device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		uint beg_index = 0;

		// �T�u�Z�b�g���̕`��
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
				// 0.99�̓J�����OOFF�ݒ�̊��S�s�����|���S�������炱���ł͕`�悵�Ȃ�
				continue;
			}
			if (abs(alpha - 0.98f) < 0.00001f)
			{
				// 0.98�̓��C�gOFF�ݒ�̊��S�s�����|���S�������炱���ł͕`�悵�Ȃ�
				continue;
			}

			// ���S�s�����̃T�u�Z�b�g��
			// �����ł͕`���Ȃ�
			if (alpha >= 1.0f)
			{
				continue;
			}

			if (alpha < 0.000000001f)
			{
				continue;
			}

			// ���C�e�B���O�L��
			device->LightEnable(0, TRUE);
			device->SetRenderState(D3DRS_LIGHTING, TRUE);

			// ���_�t�H�[�}�b�g�ݒ�
			device->SetFVF(model_fvf);

			//�}�e���A���ݒ�
			device->SetMaterial(&material_);

			//�e�N�X�`���ݒ�
			device->SetTexture(0, texture_);

			device->SetStreamSource(0, vertbuff, 0, sizeof(DxTop));
			device->SetIndices(indexbuff);

			uint index_num = use_top_num;
			uint triangle_num = index_num / 3;

			// �v���~�e�B�u�`��
			device->DrawIndexedPrimitive(
				D3DPT_TRIANGLELIST,
				0,
				0,
				index_num,
				beg_index_now,
				triangle_num);
		}

		// Z�o�b�t�@���X�V����
		device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		// �������`�斳��
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

		// ���e�X�g�֎~
		device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
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

#if 0

	bool ModelData::load_x(const TCHAR * path, const si3::Coor3 & center)
	{
		release();

		this->dxdevice = si3::Manager::get_dxdevice();

		LPD3DXBUFFER	pMtrlBuf;

		HRESULT hr = D3DXLoadMeshFromX(
			path,								// �t�@�C����
			D3DXMESH_SYSTEMMEM,					// ��ɓ����w��
			si3::Manager::get_dxdevice(),			// Direct3D�f�o�C�X
			NULL,								// ���NULL
			&pMtrlBuf,							// �}�e���A���o�b�t�@
			NULL,								// ���NULL
			&material_num,						// �}�e���A����
			&mesh);								// ���b�V���I�u�W�F�N�g
		if (FAILED(hr)) {
			return false;
		}

		LPD3DXMESH pMeshWk;
		add_normal(mesh, si3::Manager::get_dxdevice(), pMeshWk);

		if (get_table(pMeshWk, pMtrlBuf) == false)
		{
			return false;
		}

		compute_tops_center(mesh, center);

		get_material(path, pMtrlBuf, si3::Manager::get_dxdevice());

		judge_with_alpha();

		return true;
	}

	bool ModelData::load_x(const TCHAR * path)
	{
		release();

		this->dxdevice = si3::Manager::get_dxdevice();

		LPD3DXBUFFER	pMtrlBuf;

		HRESULT hr = D3DXLoadMeshFromX(
			path,								// �t�@�C����
			D3DXMESH_SYSTEMMEM,					// ��ɓ����w��
			si3::Manager::get_dxdevice(),			// Direct3D�f�o�C�X
			NULL,								// ���NULL
			&pMtrlBuf,							// �}�e���A���o�b�t�@
			NULL,								// ���NULL
			&material_num,						// �}�e���A����
			&mesh);								// ���b�V���I�u�W�F�N�g
		if (FAILED(hr)) {
			return false;
		}

		LPD3DXMESH pMeshWk;
		add_normal(mesh, si3::Manager::get_dxdevice(), pMeshWk);

		if (get_table(pMeshWk, pMtrlBuf) == false)
		{
			return false;
		}

		compute_tops(mesh);

		get_material(path, pMtrlBuf, si3::Manager::get_dxdevice());

		judge_with_alpha();

		return true;
	}

	void ModelData::add_normal(LPD3DXMESH & mesh, const LPDIRECT3DDEVICE9 & device, LPD3DXMESH & pMeshWk)
	{
		// �@���������ꍇ�͋����I�ɒǉ�
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

	bool ModelData::get_table(LPD3DXMESH & pMeshWk, LPD3DXBUFFER & pMtrlBuf)
	{
		// �����e�[�u���𐶐����邽�߂̍œK��
		HRESULT hr = mesh->Optimize(D3DXMESHOPT_ATTRSORT, NULL, NULL, NULL, NULL, &pMeshWk);
		if (SUCCEEDED(hr)) {
			mesh->Release();
			mesh = pMeshWk;
		}
		else {
			dxsaferelease(pMtrlBuf);
			return false;
		}

		// �����e�[�u���擾
		hr = mesh->GetAttributeTable(NULL, &attribute_num);
		if (FAILED(hr)) {
			dxsaferelease(pMtrlBuf);
			dxsaferelease(mesh);
			return false;
		}
		attribute = new D3DXATTRIBUTERANGE[attribute_num];
		hr = mesh->GetAttributeTable(attribute, &attribute_num);

		return true;
	}

	void ModelData::compute_tops(LPD3DXMESH & mesh)
	{
		// ���_�o�b�t�@�Œ�
		LPD3DXVECTOR3 pVtx;
		mesh->LockVertexBuffer(0, (LPVOID*)&pVtx);
		// ���E�{�b�N�X����
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
		// ���E������
		fRadius = 0.0f;
		float fR;
		pV = pVtx;
		for (DWORD i = 0; i < dwVtx; i++) {
			fR = D3DXVec3Length(&(*pV - vCenter));
			if (fRadius < fR)
				fRadius = fR;
			pV = (LPD3DXVECTOR3)((LPBYTE)pV + dwStroke);
		}
		// ���_�o�b�t�@���
		mesh->UnlockVertexBuffer();
	}
	void ModelData::compute_tops_center(LPD3DXMESH & mesh, const Coor3 & center)
	{
		// ���_�o�b�t�@�Œ�
		LPD3DXVECTOR3 pVtx;
		mesh->LockVertexBuffer(0, (LPVOID*)&pVtx);
		// ���E�{�b�N�X����
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
		// ���E������
		fRadius = 0.0f;
		float fR;
		pV = pVtx;
		for (DWORD i = 0; i < dwVtx; i++) {
			fR = D3DXVec3Length(&(*pV - vCenter));
			if (fRadius < fR)
				fRadius = fR;
			pV = (LPD3DXVECTOR3)((LPBYTE)pV + dwStroke);
		}
		// ���_�o�b�t�@���
		mesh->UnlockVertexBuffer();
	}

	void ModelData::get_material(const TCHAR * path, LPD3DXBUFFER	pMtrlBuf, const LPDIRECT3DDEVICE9 & device)
	{
		TCHAR			szDir[_MAX_DIR];
		TCHAR			szSaveDir[_MAX_DIR];

		// �J�����g�f�B���N�g����X�t�@�C���̂���t�H���_�Ɉړ�
		_tsplitpath(path, NULL, szDir, NULL, NULL);
		if (szDir[0]) {	// �t�H���_���Ύw�肠��?
			GetCurrentDirectory(_countof(szSaveDir), szSaveDir);
			SetCurrentDirectory(szDir);
		}

		// �}�e���A���̒��o
		D3DXMATERIAL* pMtrl = (D3DXMATERIAL*)pMtrlBuf->GetBufferPointer();
		material = new D3DMATERIAL9[material_num];
		ppTexture = new LPDIRECT3DTEXTURE9[material_num];
		for (DWORD i = 0; i < material_num; i++) {
			material[i] = pMtrl[i].MatD3D;
			// X�t�@�C���ɂ͊��F�������A�g�U�F�ŏ㏑�����Ă���.
			material[i].Ambient = material[i].Diffuse;
			ppTexture[i] = NULL;
			if (pMtrl[i].pTextureFilename && pMtrl[i].pTextureFilename[0]) {
				HRESULT hr = D3DXCreateTextureFromFileA(device, pMtrl[i].pTextureFilename, &ppTexture[i]);
				if (FAILED(hr)) {
					break;
				}
			}
		}

		// �J�����g�f�B���N�g�������ɖ߂�
		if (szDir[0]) {	// �t�H���_���Ύw�肠��?
			SetCurrentDirectory(szSaveDir);
		}

		pMtrlBuf->Release();
	}

	void ModelData::judge_with_alpha()
	{
		for (DWORD i = 0; i < attribute_num; i++) {
			DWORD id = attribute[i].AttribId;
			if (material[id].Diffuse.a < 1.0f) {
				with_alpha = true;
				break;
			}
		}
	}




	void ModelData::compute_world_mat(D3DXMATRIX & mat, const ModelCoor & coor) const
	{
		D3DXMATRIX matRot, matMove;
		D3DXMatrixRotationYawPitchRoll(&matRot, coor.radian_y, coor.radian_x, coor.radian_z);
		D3DXMatrixTranslation(&matMove, coor.x, coor.y, coor.z);
		D3DXMatrixMultiply(&mat, &matRot, &matMove);

	}
#endif

}

#pragma warning(pop)