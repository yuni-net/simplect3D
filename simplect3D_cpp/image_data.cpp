#include <si3_ImageData.h>
#include <si3_Manager.h>
#include <si3_utility.h>
#include <si3_Top4.h>
#include <si3_Diffuse.h>
#include <si3_ModelCoor.h>



static const WORD LAND_FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;



bool si3::ImageData::init_vertex(
	LPDIRECT3DDEVICE9 device,			// in
	int top_num_x,						// in
	int top_num_y,						// in
	int top_num,						// in
	int width,							// in
	int height,							// in
	int piece_num_x,					// in
	int piece_num_y,					// in
	IDirect3DVertexBuffer9 ** vertbuff)	// out
{
	HRESULT	hr;

	// ���_���i�[�o�b�t�@���쐬
	hr = device->CreateVertexBuffer(
		sizeof(DxTop)* top_num,
		D3DUSAGE_WRITEONLY,
		LAND_FVF,
		D3DPOOL_MANAGED,
		vertbuff,
		NULL);

	if (FAILED(hr)) return false;

	// �o�b�t�@�����b�N�����ď������݂��J�n����
	DxTop * vert_arr = nullptr;
	hr = (*vertbuff)->Lock(0, 0, fw::pointer_cast<void **>(&vert_arr), 0);
	if (FAILED(hr)) return false;

	/*
	* 1.�ł�����̒��_��񂩂�i�[���Ă䂭
	* 2.������E�Ɍ������ď��X�ɒ��_�����i�[���Ă䂭
	* 3.��ԉE�܂Ŋi�[���I������A����̍s�Ɉړ�����
	* 4.2��3���A�Ō�̍s�ɂȂ�܂ŌJ��Ԃ�
	*
	*/
	int target_index = 0;
	for (int index_y = 0; index_y < top_num_y; ++index_y)		// ��̍s���牺�̍s�֒��_�����i�[���Ă䂭
	{
		for (int index_x = 0; index_x < top_num_x; ++index_x)	// ���̗񂩂�E�̗�֒��_�����i�[���Ă䂭
		{
			DxTop & vertex = vert_arr[target_index];

			vertex.pos.x = static_cast<float>(width*index_x) / piece_num_x - width / 2.0f;
			vertex.pos.y = static_cast<float>(height*(top_num_y - 1 - index_y)) / piece_num_y - height / 2.0f;
			vertex.pos.z = 0.0f;
			vertex.normal.x = 0.0f;
			vertex.normal.y = 0.0f;
			vertex.normal.z = -1.0f;
			vertex.u = static_cast<float>(index_x) / piece_num_x;
			vertex.v = static_cast<float>(index_y) / piece_num_y;

			++target_index;
		}
	}

	// �o�b�t�@���A�����b�N���ď������݂��I������
	(*vertbuff)->Unlock();

	return true;
}

bool si3::ImageData::init_index(
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
	WORD * index_arr = nullptr;
	hr = (*indexbuff)->Lock(0, 0, fw::pointer_cast<void **>(&index_arr), 0);
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
			index_arr[target_index] = top_num_x*(index_y + 1) + index_x;	// �܂������̃C���f�b�N�X���i�[
			++target_index;
			index_arr[target_index] = top_num_x*(index_y) +index_x;			// ���ɏ㑤�̃C���f�b�N�X���i�[
			++target_index;
		}

		// �k�ރ|���S���p�̃C���f�b�N�X��ǉ�����
		const int piece_index_last = piece_num_y - 1;	// �Ō�̍s��index_y
		if (index_y < piece_index_last)	// �Ō�̍s�����O�̍s����������s
		{
			index_arr[target_index] = top_num_x*(index_y) +top_num_x - 1;	// ���̍s�̍Ō�̃C���f�b�N�X�Ɠ����C���f�b�N�X��ǉ�
			++target_index;
			index_arr[target_index] = top_num_x*(index_y + 1 + 1) + 0;		// ���̍s�̍ŏ��̃C���f�b�N�X�Ɠ����C���f�b�N�X��ǉ�
			++target_index;
		}
	}

	// �o�b�t�@���A�����b�N���ď������ݏI��
	(*indexbuff)->Unlock();

	return true;
}

bool si3::ImageData::load(
	LPDIRECT3DDEVICE9 device,			// in
	const TCHAR * path,					// in
	LPDIRECT3DTEXTURE9 * texture,		// out
	IDirect3DVertexBuffer9 ** vertbuff,	// out
	IDirect3DIndexBuffer9 ** indexbuff,	// out
	int * index_num,					// out
	int * triangle_num,					// out
	float piece_size,					// in
	uint & width,						// out
	uint & height)					// out
{

	dxsaferelease(*texture);
	dxsaferelease(*vertbuff);
	dxsaferelease(*indexbuff);

	HRESULT hr;

	// �e�N�X�`���̃T�C�Y�𒲂ׂċL������
	D3DXIMAGE_INFO info;
	hr = D3DXGetImageInfoFromFile(path, &info);
	if (FAILED(hr)) return false;
	width = info.Width;
	height = info.Height;

	int piece_num_x = static_cast<int>(width / piece_size + 1);	// �|���S�����i�q��ɕ����Ǘ������Ƃ��̗�
	int piece_num_y = static_cast<int>(height / piece_size + 1);	// �|���S�����i�q��ɕ����Ǘ������Ƃ��̍s��
	int piece_num = piece_num_x * piece_num_y;	// �|���S�����i�q��ɕ����Ǘ������Ƃ��̒f�Ђ̌�

	int top_num_x = piece_num_x + 1;			// �|���S�����i�q��ɕ����Ǘ������Ƃ��̒��_�̗�
	int top_num_y = piece_num_y + 1;			// �|���S�����i�q��ɕ����Ǘ������Ƃ��̒��_�̍s��
	int top_num = top_num_x*top_num_y;			// �|���S�����i�q��ɕ����Ǘ������Ƃ��̒��_�̌�

	const int top_per_row = 2 * top_num_x;				// ��s�ɂ����̒��_�����g�p���邩
	const int top_of_grid = top_per_row * piece_num_y;	// �i�q��̒��_���̍��v
	const int top_for_degen_per_row = 2;				// ��s�ɂ��K�v�ȏk�ރ|���S���p�̒��_���̐�
	const int top_for_degen = top_for_degen_per_row * (piece_num_y - 1);// �k�ރ|���S���p�̒��_���̍��v
	*index_num = top_of_grid + top_for_degen;			// ���_�C���f�b�N�X���̌�
	*triangle_num = *index_num - 2;						// �O�p�|���S���̍��v

	// �e�N�X�`���쐬
	hr = D3DXCreateTextureFromFileEx(
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
		texture);
	if (FAILED(hr)) return false;

	bool result;

	// ���_�o�b�t�@�쐬�A���_�f�[�^�ݒ�
	result = init_vertex(
		device,
		top_num_x,
		top_num_y,
		top_num,
		static_cast<int>(width),
		static_cast<int>(height),
		piece_num_x,
		piece_num_y,
		vertbuff);
	if (result == false) return false;

	// ���_�C���f�b�N�X�o�b�t�@�쐬�A���_�C���f�b�N�X�f�[�^�ݒ�
	result = init_index(
		device,
		*index_num,
		indexbuff,
		top_num_x,
		top_num_y,
		top_num,
		piece_num_x,
		piece_num_y);
	if (result == false) return false;

	return true;
}


bool si3::ImageData::draw(
	LPDIRECT3DDEVICE9 device,
	const D3DXMATRIX & world_mat,
	LPDIRECT3DTEXTURE9 texture,
	IDirect3DVertexBuffer9 * vertbuff,
	IDirect3DIndexBuffer9 * indexbuff,
	int index_num,
	int triangle_num,
	const D3DMATERIAL9 & material) const
{
	// ���[���h�ϊ��s��ݒ�
	device->SetTransform(D3DTS_WORLD, &world_mat);

	// ���_�t�H�[�}�b�g�ݒ�
	device->SetFVF(LAND_FVF);

	//�}�e���A���ݒ�
	device->SetMaterial(&material);

	//�e�N�X�`���ݒ�
	device->SetTexture(0, texture);

	device->SetStreamSource(0, vertbuff, 0, sizeof(DxTop));
	device->SetIndices(indexbuff);


	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	//device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//device->SetRenderState(D3DRS_FILLMODE, D3DFILL_POINT);

	// �v���~�e�B�u�`��
	device->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		index_num,
		0,
		triangle_num);

	return true;
}

namespace si3
{
	ImageData::ImageData()
	{
		construct();
	}
	ImageData::ImageData(const Manager & manageri, const char * path, float piece_size)
	{
		construct();
		load(manageri, path, piece_size);
	}
	ImageData::ImageData(const Manager & manageri, const std::string & path, float piece_size)
	{
		construct();
		load(manageri, path, piece_size);
	}

	bool ImageData::load(const Manager & manageri, const char * path, float piece_size)
	{
		device = manageri.get_dxdevice();

		load(
			device,
			path,
			&texture,
			&vertbuff,
			&indexbuff,
			&index_num,
			&triangle_num,
			piece_size,
			width_,
			height_);

		return true;
	}
	bool ImageData::load(const Manager & manageri, const std::string & path, float piece_size)
	{
		return load(manageri, path.c_str(), piece_size);
	}

	uint ImageData::width() const
	{
		return width_;
	}
	uint ImageData::height() const
	{
		return height_;
	}

	void ImageData::compute_world_mat(D3DXMATRIX & mat, const ModelCoor & coor)
	{
		D3DXMATRIX matRot, matMove;
		D3DXMatrixRotationYawPitchRoll(&matRot, coor.radian_y, coor.radian_x, coor.radian_z);
		D3DXMatrixTranslation(&matMove, coor.x, coor.y, coor.z);
		D3DXMatrixMultiply(&mat, &matRot, &matMove);
	}

	bool ImageData::draw_no_alpha(const D3DXMATRIX & world_mat, const D3DMATERIAL9 & material, bool is_culling_on) const
	{
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

		if (is_culling_on)
		{
			device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		}
		else
		{
			device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		}

		return draw(world_mat, material);
	}
	bool ImageData::draw_alpha(const D3DXMATRIX & world_mat, const D3DMATERIAL9 & material, int blend_mode, bool is_culling_on) const
	{
		// �������\�����\�ɂ���(���u�����f�B���O�L����)
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		// ���S�����������������܂Ȃ�(���e�X�g�L����)
		device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		device->SetRenderState(D3DRS_ALPHAREF, 0);
		device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		// �}�e���A���ƃe�N�X�`����rgba�����|�����킹��ݒ�
		device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
		device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);

		if (is_culling_on)
		{
			device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		}
		else
		{
			device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		}

		// Z�o�b�t�@���X�V���Ȃ�
		device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		// ���Z�����L����
		if (blend_mode == add_blend)
		{
			device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		}

		// ���Z�����L����
		if (blend_mode == sub_blend)
		{
			device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
			device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		}

		bool result = draw(world_mat, material);

		// ���Z��������
		if (blend_mode == add_blend)
		{
			device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}

		// ���Z��������
		if (blend_mode == sub_blend)
		{
			device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}

		// Z�o�b�t�@���X�V����
		device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		return result;
	}




	bool ImageData::draw(const D3DXMATRIX & world_mat, const D3DMATERIAL9 & material) const
	{
		draw(
			device,
			world_mat,
			texture,
			vertbuff,
			indexbuff,
			index_num,
			triangle_num,
			material);

		return true;
	}


	void ImageData::release()
	{
		dxsaferelease(texture);
		dxsaferelease(vertbuff);
		dxsaferelease(indexbuff);
	}

	ImageData::~ImageData()
	{
		release();
	}



	void ImageData::construct()
	{
		texture = nullptr;
		vertbuff = nullptr;
		indexbuff = nullptr;
	}




}


