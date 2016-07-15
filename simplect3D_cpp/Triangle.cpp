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

		// ���_���i�[�o�b�t�@���쐬
		hr = device->CreateVertexBuffer(
			sizeof(Top)* 3,
			D3DUSAGE_WRITEONLY,
			fvf,
			D3DPOOL_MANAGED,
			&vertbuff,
			NULL);

		if (FAILED(hr)) return;

		// �o�b�t�@�����b�N�����ď������݂��J�n����
		Top * vert_arr = nullptr;
		hr = vertbuff->Lock(0, 0, fw::pointer_cast<void **>(&vert_arr), 0);
		if (FAILED(hr)) return;

		vert_arr[0].pos = a.dxvec3();
		vert_arr[0].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vert_arr[1].pos = b.dxvec3();
		vert_arr[1].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vert_arr[2].pos = c.dxvec3();
		vert_arr[2].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// �o�b�t�@���A�����b�N���ď������݂��I������
		vertbuff->Unlock();
	}

	void Triangle::draw_no_alpha() const
	{
		LPDIRECT3DDEVICE9 device = si3::Manager::get_dxdevice();

		// Z�o�b�t�@���X�V����
		device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		// ���e�X�g�֎~
		device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

		// �������\���֎~(���u�����f�B���O������)
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

		// �}�e���A���ƃe�N�X�`����rgba�����|�����킹��ݒ�
		device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTA_DIFFUSE);
		device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);

		// ���[���h�ϊ��s��ݒ�
		D3DXMATRIX world_mat = Matrix().parallel(x,y,z).dxmat();
		device->SetTransform(D3DTS_WORLD, &world_mat);

		// ���_�t�H�[�}�b�g�ݒ�
		device->SetFVF(fvf);

		//�}�e���A���ݒ�
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

		// �v���~�e�B�u�`��
		device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 1);
	}

	void Triangle::draw_alpha() const
	{
		// Nothing
	}







}