#if 1

#include <si3_utility.h>
#include <si3_SkydomeData.h>
#include <si3_saferelease.h>

static const WORD SPHERE_FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

namespace si3
{
	SkydomeData::SkydomeData()
	{
		construct();
	}
	SkydomeData::SkydomeData(LPDIRECT3DDEVICE9 device, const char * path, float roughness)
	{
		construct();
		load(device, path, roughness);
	}

	void SkydomeData::load(LPDIRECT3DDEVICE9 device, const char * path, float roughness)
	{
		this->device = device;

		release();

		const float diameter = 2.0f;
		const float width = diameter * pi;
		const float height = diameter;

		int piece_num_x = static_cast<int>(width / roughness) + 1;	// �|���S�����i�q��ɕ����Ǘ������Ƃ��̗�
		int piece_num_y = static_cast<int>(height / roughness) + 1;	// �|���S�����i�q��ɕ����Ǘ������Ƃ��̍s��
		int piece_num = piece_num_x * piece_num_y;	// �|���S�����i�q��ɕ����Ǘ������Ƃ��̒f�Ђ̌�

		int top_num_x = piece_num_x + 1;			// �|���S�����i�q��ɕ����Ǘ������Ƃ��̒��_�̗�
		int top_num_y = piece_num_y + 1;			// �|���S�����i�q��ɕ����Ǘ������Ƃ��̒��_�̍s��
		int top_num = top_num_x*top_num_y;			// �|���S�����i�q��ɕ����Ǘ������Ƃ��̒��_�̌�

		const int top_per_row = 2 * top_num_x;				// ��s�ɂ����̒��_�����g�p���邩
		const int top_of_grid = top_per_row * piece_num_y;	// �i�q��̒��_���̍��v
		const int top_for_degen_per_row = 2;				// ��s�ɂ��K�v�ȏk�ރ|���S���p�̒��_���̐�
		const int top_for_degen = top_for_degen_per_row * (piece_num_y - 1);// �k�ރ|���S���p�̒��_���̍��v
		index_num = top_of_grid + top_for_degen;			// ���_�C���f�b�N�X���̌�
		triangle_num = index_num - 2;						// �O�p�|���S���̍��v

		// �e�N�X�`���쐬
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

		bool result;

		// ���_�o�b�t�@�쐬�A���_�f�[�^�ݒ�
		result = init_vertex(
			device,
			top_num_x,
			top_num_y,
			top_num,
			width,
			height,
			piece_num_x,
			piece_num_y,
			&vertbuff);
		if (result == false) return;

		// ���_�C���f�b�N�X�o�b�t�@�쐬�A���_�C���f�b�N�X�f�[�^�ݒ�
		result = init_index(
			device,
			index_num,
			&indexbuff,
			top_num_x,
			top_num_y,
			top_num,
			piece_num_x,
			piece_num_y);
		if (result == false) return;
	}

	bool SkydomeData::init_vertex(
		LPDIRECT3DDEVICE9 device,			// in
		int top_num_x,						// in
		int top_num_y,						// in
		int top_num,						// in
		float width,						// in
		float height,						// in
		int piece_num_x,					// in
		int piece_num_y,					// in
		IDirect3DVertexBuffer9 ** vertbuff)	// out
	{
		HRESULT	hr;

		// ���_���i�[�o�b�t�@���쐬
		hr = device->CreateVertexBuffer(
			sizeof(land_vertex)* top_num,
			D3DUSAGE_WRITEONLY,
			SPHERE_FVF,
			D3DPOOL_MANAGED,
			vertbuff,
			NULL);

		if (FAILED(hr)) return false;

		// �o�b�t�@�����b�N�����ď������݂��J�n����
		land_vertex * pVtx = nullptr;
		hr = (*vertbuff)->Lock(0, 0, fw::pointer_cast<void **>(&pVtx), 0);
		if (FAILED(hr)) return false;

		/*
		* 1.�ł�����̒��_��񂩂�i�[���Ă䂭
		* 2.������E�Ɍ������ď��X�ɒ��_�����i�[���Ă䂭
		* 3.��ԉE�܂Ŋi�[���I������A����̍s�Ɉړ�����
		* 4.2��3���A�Ō�̍s�ɂȂ�܂ŌJ��Ԃ�
		*
		*/
		const float piece_width = 1.0f / top_num_x;
		const float piece_height = 1.0f / top_num_y;
		int target_index = 0;
		for (int index_y = 0; index_y < top_num_y; ++index_y)		// ��̍s���牺�̍s�֒��_�����i�[���Ă䂭
		{
			if (index_y == top_num_y - 1)
			{
				index_y = top_num_y - 1;
			}
			for (int index_x = 0; index_x < top_num_x; ++index_x)	// ���̗񂩂�E�̗�֒��_�����i�[���Ă䂭
			{
				land_vertex & vertex = pVtx[target_index];

				float theta = 2.0f*pi*index_y / piece_num_y;
				float cos_value = cos(theta);
				cos_value = cos_value*0.5f;
				vertex.pos.y = height*cos_value;
			//	vertex.pos.y = height*sin(theta);

				float radian = 2*pi * index_x/piece_num_x;

				float radius = height*0.5f * sin(theta);

				vertex.pos.x = cos(radian)*radius;
				vertex.pos.z = sin(radian)*radius;

				// �@���x�N�g���͌��_����xyz�܂ł̃x�N�g����P�ʃx�N�g���ɒ��������̂ƈ�v����
				// �����ŒP�ʃx�N�g�����v�Z���Avertex.normal�ɑ�����Ă���
				vector_to_unit_vector(vertex.pos, vertex.normal);

				vertex.u = piece_width*index_x;
				vertex.v = piece_height*index_y;

				++target_index;
			}
		}

		// �o�b�t�@���A�����b�N���ď������݂��I������
		(*vertbuff)->Unlock();

		return true;
	}

	bool SkydomeData::init_index(
		LPDIRECT3DDEVICE9 device,			// in
		int index_num,						// in
		IDirect3DIndexBuffer9 ** indexbuff,	// out
		int top_num_x,						// in
		int top_num_y,						// in
		int top_num,						// in
		int piece_num_x,					// in
		int piece_num_y)					// in
	{
		HRESULT hr;

		// ���_�C���f�b�N�X�o�b�t�@�쐬
		hr = device->CreateIndexBuffer(
			sizeof(WORD)* index_num,
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			indexbuff,
			NULL);
		if (FAILED(hr)) return false;

		// �o�b�t�@�����b�N���ď������݊J�n
		WORD * pIndex = nullptr;
		hr = (*indexbuff)->Lock(0, 0, fw::pointer_cast<void **>(&pIndex), 0);
		if (FAILED(hr)) return false;

		/*
		* N����ɒ��_�����o�����悤�ɃC���f�b�N�X��ݒ肵�Ă䂭
		* �s�̉E�[�܂ŗ�����A�k�ރ|���S���p�ɓ����C���f�b�N�X��������ƁA
		* ���̃C���f�b�N�X��������ǉ����Ă����B
		* ������Ō�̍s�ɂȂ�܂ŌJ��Ԃ��B
		* �������Ō�̍s�����͏k�ރ|���S���p�̃C���f�b�N�X��ǉ����Ȃ��B
		*
		*/
		int target_index = 0;
		for (int index_y = 0; index_y < piece_num_y; ++index_y)		// �ォ�牺�̍s�Ɍ������ăC���f�b�N�X���i�[
		{
			for (int index_x = 0; index_x < top_num_x; ++index_x)	// ������E�̗�Ɍ������ăC���f�b�N�X���i�[
			{
				pIndex[target_index] = top_num_x*(index_y + 1) + index_x;	// �܂������̃C���f�b�N�X���i�[
				++target_index;
				pIndex[target_index] = top_num_x*(index_y)+index_x;			// ���ɏ㑤�̃C���f�b�N�X���i�[
				++target_index;
			}

			// �k�ރ|���S���p�̃C���f�b�N�X��ǉ�����
			const int piece_index_last = piece_num_y - 1;	// �Ō�̍s��index_y
			if (index_y < piece_index_last)	// �Ō�̍s�����O�̍s����������s
			{
				pIndex[target_index] = top_num_x*(index_y)+top_num_x - 1;	// ���̍s�̍Ō�̃C���f�b�N�X�Ɠ����C���f�b�N�X��ǉ�
				++target_index;
				pIndex[target_index] = top_num_x*(index_y + 1 + 1) + 0;		// ���̍s�̍ŏ��̃C���f�b�N�X�Ɠ����C���f�b�N�X��ǉ�
				++target_index;
			}
		}

		// �o�b�t�@���A�����b�N���ď������ݏI��
		(*indexbuff)->Unlock();

		return true;
	}

	void SkydomeData::draw(const D3DXMATRIX & world_mat) const
	{
		// ���[���h�ϊ��s��ݒ�
		device->SetTransform(D3DTS_WORLD, &world_mat);

		// ���_�t�H�[�}�b�g�ݒ�
		device->SetFVF(SPHERE_FVF);


		device->SetTexture(0, texture);

		device->SetStreamSource(0, vertbuff, 0, sizeof(land_vertex));
		device->SetIndices(indexbuff);


		// �v���~�e�B�u�`��
		device->DrawIndexedPrimitive(
			D3DPT_TRIANGLESTRIP,
			0,
			0,
			index_num,
			0,
			triangle_num);

	}


	SkydomeData::~SkydomeData()
	{
		release();
	}

	void SkydomeData::construct()
	{
		device = nullptr;
		vertbuff = nullptr;
		indexbuff = nullptr;
		texture = nullptr;
	}
	void SkydomeData::release()
	{
		dxsaferelease(vertbuff);
		dxsaferelease(indexbuff);
		dxsaferelease(texture);
	}


}

#endif