#include <si3_bunisher.h>
#include <si3_Manager.h>
#include <si3_Model.h>
#include <si3_utility.h>
#include <si3_ModelDustConverter.h>

const float alpha_speed = 0.01f;

namespace si3
{
	static const DWORD model_fvf = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;



	Bunisher::Bunisher(const Model & model_)
	{
		construct();
		init(model_);
	}

	bool Bunisher::create_buffer(unsigned long buffer_size)
	{
		release();

		// ���_���i�[�o�b�t�@���쐬
		HRESULT hr = device->CreateVertexBuffer(
			sizeof(DxTop)* buffer_size,
			D3DUSAGE_WRITEONLY,
			model_fvf,
			D3DPOOL_MANAGED,
			&listbuff,
			NULL);

		if (FAILED(hr)) return false;

		return true;
	}


	void Bunisher::init(const Model & model_)
	{
		typedef unsigned short ushort;

		device = si3::Manager::get_dxdevice();

		const ModelData & modeld = model_.model_data();
		unsigned long top_num = modeld.index_num();
		triangle_num = top_num/3;
		bunish_data_list.resize(triangle_num);
		create_buffer(top_num);

		DxTop * planebuff = modeld.lock_top_buffer();
		ushort * indexbuff = modeld.lock_index_buffer();

		convert_without_index(planebuff, indexbuff, modeld.index_num());

		modeld.unlock_top_buffer();
		modeld.unlock_index_buffer();

		alpha = 1.0f;
	}



	void Bunisher::bunish()
	{
		DxTop * top_header;
		listbuff->Lock(0, 0, fw::pointer_cast<void **>(&top_header), 0);
		if (top_header == nullptr)
		{
			return;
		}

		uint top_No = 0;
		auto iter = bunish_data_list.begin();

		while (iter != bunish_data_list.end())
		{
			BunishData & bunishd = *iter;
			bunishd.bunish();

			top_header[top_No + 0].pos = bunishd.compute_local_pos(0);
			top_header[top_No + 1].pos = bunishd.compute_local_pos(1);
			top_header[top_No + 2].pos = bunishd.compute_local_pos(2);

			top_No += 3;
			++iter;
		}

		listbuff->Unlock();

		alpha -= alpha_speed;
#ifdef NDEBUG
		bunish_data_list.sort();	// �~���Ƀ\�[�g�����
#endif
	}





	Bunisher::Bunisher()
	{
		construct();
	}

	Bunisher::~Bunisher()
	{
		release();
	}

	void Bunisher::draw_no_alpha() const
	{
		// Nothing
	}

	void Bunisher::draw_alpha() const
	{
		// ���[���h�ϊ��s��ݒ�
		device->SetTransform(D3DTS_WORLD, Matrix().dxpointer());

		// Z�o�b�t�@���X�V���Ȃ�
		device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		// ���e�X�g�L��
		device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

		// �������`��L��
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		// ���C�e�B���O�L��
		device->LightEnable(0, TRUE);
		device->SetRenderState(D3DRS_LIGHTING, TRUE);

		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	// �J�����O����

		// ���_�t�H�[�}�b�g�ݒ�
		device->SetFVF(model_fvf);

		//�e�N�X�`���ݒ�
		device->SetTexture(0, NULL);

		D3DMATERIAL9 material_;
		material_.Diffuse = { 0.28f, 0.88f, 0.88f, alpha };
		material_.Ambient = {0.0f,0.0f,0.0f,0.0f};
		material_.Emissive = { 0.0f, 0.0f, 0.0f, 0.0f };
		material_.Specular = { 1.0f, 1.0f, 1.0f, alpha };
		material_.Power = 1.0f;

		//�}�e���A���ݒ�
		device->SetMaterial(&material_);

		device->SetStreamSource(0, listbuff, 0, sizeof(DxTop));



		device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, triangle_num);



		// Z�o�b�t�@���X�V����
		device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		// �������`�斳��
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

		// ���e�X�g�֎~
		device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}


	void Bunisher::construct()
	{
		device = nullptr;
		listbuff = nullptr;
	}

	void Bunisher::release()
	{
		dxsaferelease(listbuff);
	}


	void Bunisher::convert_without_index(DxTop * planebuff, unsigned short * indexbuff, unsigned long index_num)
	{
		DxTop * top_header;
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
			BunishData & bunishd = *iter;

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






}