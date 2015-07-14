#include <anime_data.h>
#include <Manager.h>
#include <utility.h>
#include <top4.h>
#include <diffuse.h>
#include <model_coor.h>

// max_vertex = 11*11
// �ȃ����O�L���i�\�̂ݕ`��j
// device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);�J�����O�L��
// device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);�J�����O����


static const WORD LAND_FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

struct land_vertex
{
	D3DVECTOR   pos;
	D3DVECTOR   normal;
	float       u, v;
};


bool si3::anime_data::init_vertex(
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
		sizeof(land_vertex)* top_num,
		D3DUSAGE_WRITEONLY,
		LAND_FVF,
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
	int target_index = 0;
	for (int index_y = 0; index_y < top_num_y; ++index_y)		// ��̍s���牺�̍s�֒��_�����i�[���Ă䂭
	{
		for (int index_x = 0; index_x < top_num_x; ++index_x)	// ���̗񂩂�E�̗�֒��_�����i�[���Ă䂭
		{
			land_vertex & vertex = pVtx[target_index];

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

bool si3::anime_data::init_index(
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


bool si3::anime_data::draw(
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

	device->SetStreamSource(0, vertbuff, 0, sizeof(land_vertex));
	device->SetIndices(indexbuff);

	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

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
	anime_data::grid::grid(uint uWidth, uint uHeight, float piece_size)
	{
		// todo
	}
	int anime_data::grid::piece_num_x() const
	{
		return piece_num_x_;
	}
	int anime_data::grid::piece_num_y() const
	{
		return piece_num_y_;
	}
	int anime_data::grid::piece_num() const
	{
		return piece_num_;
	}
	int anime_data::grid::top_num_x() const
	{
		return top_num_x_;
	}
	int anime_data::grid::top_num_y() const
	{
		return top_num_y_;
	}
	int anime_data::grid::top_num() const
	{
		return top_num_;
	}
	int anime_data::grid::index_num() const
	{
		return index_num_;
	}
	int anime_data::grid::triangle_num() const
	{
		return triangle_num_;
	}


	anime_data::anime_data()
	{
		construct();
	}
	anime_data::anime_data(const Manager & manageri, const char * path, float piece_size, uint uPieceWidth, uint uPieceHeight, uint uEnableNum)
	{
		construct();
		load(manageri, path, piece_size, uPieceWidth, uPieceHeight, uEnableNum);
	}
	anime_data::anime_data(const Manager & manageri, const std::string & path, float piece_size, uint uPieceWidth, uint uPieceHeight, uint uEnableNum)
	{
		construct();
		load(manageri, path, piece_size, uPieceWidth, uPieceHeight, uEnableNum);
	}

	bool anime_data::load(const Manager & manageri, const char * path, float piece_size, uint uPieceWidth, uint uPieceHeight, uint uEnableNum)
	{
		release();

		device = manageri.get_dxdevice();
		this->uEnableNum = uEnableNum;
		Width = uPieceWidth;	// 1�R�}�̉���
		Height = uPieceHeight;	// 1�R�}�̏c��

		HRESULT hr;

		// �e�N�X�`���̃T�C�Y�𒲂ׂċL������
		D3DXIMAGE_INFO info;
		hr = D3DXGetImageInfoFromFile(path, &info);
		if (FAILED(hr)) return false;

		uint all_width = info.Width;	// �摜�S�̂̉���
		uint all_height = info.Height;	// �摜�S�̂̏c��

		uint frame_num_x = all_width / Width;		// ���ɉ��R�}�i�[����Ă��邩
		uint frame_num_y = all_height / Height;		// �c�ɉ��R�}�i�[����Ă��邩
		uint frame_num = frame_num_x*frame_num_y;	// �摜�S�̂ŉ��R�}�i�[����Ă��邩

		int piece_num_x = static_cast<int>(Width / piece_size)+1;			// 1�R�}�͈̔͂ŁA�i�q��ɕ��������Ƃ��̗�
		float whole_piece_width = static_cast<float>(Width) / piece_num_x;	// �i�q��ɕ��������Ƃ���1�s�[�X�̉���
		int piece_num_y = static_cast<int>(Height / piece_size)+1;			// 1�R�}�͈̔͂ŁA�i�q��ɕ��������Ƃ��̍s��
		float whole_piece_height = static_cast<float>(Height) / piece_num_y;// �i�q��ɕ��������Ƃ���1�s�[�X�̏c��

		int whole_piece_num_x = piece_num_x*frame_num_x;	// �|���S�����i�q��ɕ����Ǘ������Ƃ��̗�
		int whole_piece_num_y = piece_num_y*frame_num_y;	// �|���S�����i�q��ɕ����Ǘ������Ƃ��̍s��
		int whole_piece_num = whole_piece_num_x * whole_piece_num_y;	// �|���S�����i�q��ɕ����Ǘ������Ƃ��̒f�Ђ̌�

		int whole_top_num_x = whole_piece_num_x + 1;			// �|���S�����i�q��ɕ����Ǘ������Ƃ��̒��_�̗�
		int whole_top_num_y = whole_piece_num_y + 1;			// �|���S�����i�q��ɕ����Ǘ������Ƃ��̒��_�̍s��
		int whole_top_num = whole_top_num_x*whole_top_num_y;			// �|���S�����i�q��ɕ����Ǘ������Ƃ��̒��_�̌�

		const int whole_top_per_row = 2 * whole_top_num_x;				// ��s�ɂ����̒��_�����g�p���邩
		const int whole_top_of_grid = whole_top_per_row * whole_piece_num_y;	// �i�q��̒��_���̍��v
		const int whole_top_for_degen_per_row = 2;				// ��s�ɂ��K�v�ȏk�ރ|���S���p�̒��_���̐�
		const int whole_top_for_degen = whole_top_for_degen_per_row * (whole_piece_num_y - 1);// �k�ރ|���S���p�̒��_���̍��v
		const int whole_index_num = whole_top_of_grid + whole_top_for_degen;			// ���_�C���f�b�N�X���̌�

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
			&texture);
		if (FAILED(hr)) return false;

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

		const int c_nTotalTopNum = frame_num*top_num;

		// ���_���i�[�o�b�t�@���쐬
		hr = device->CreateVertexBuffer(
			sizeof(land_vertex)* c_nTotalTopNum,
			D3DUSAGE_WRITEONLY,
			LAND_FVF,
			D3DPOOL_MANAGED,
			&vertbuff,
			NULL);

		if (FAILED(hr)) return false;

		// �o�b�t�@�����b�N�����ď������݂��J�n����
		land_vertex * pVtx = nullptr;
		hr = vertbuff->Lock(0, 0, fw::pointer_cast<void **>(&pVtx), 0);
		if (FAILED(hr)) return false;

		/*
		* 1.�ł�����̒��_��񂩂�i�[���Ă䂭
		* 2.������E�Ɍ������ď��X�ɒ��_�����i�[���Ă䂭
		* 3.��ԉE�܂Ŋi�[���I������A����̍s�Ɉړ�����
		* 4.2��3���A�Ō�̍s�ɂȂ�܂ŌJ��Ԃ�
		*
		*/
		int target_index = 0;
		for (uint uFrameYNo = 0; uFrameYNo < frame_num_y; ++uFrameYNo)
		{
			for (uint uFrameXNo = 0; uFrameXNo < frame_num_x; ++uFrameXNo)
			{
				for (int index_y = 0; index_y < top_num_y; ++index_y)		// ��̍s���牺�̍s�֒��_�����i�[���Ă䂭
				{
					for (int index_x = 0; index_x < top_num_x; ++index_x)	// ���̗񂩂�E�̗�֒��_�����i�[���Ă䂭
					{
						land_vertex & vertex = pVtx[target_index];

						float x = whole_piece_width*index_x;
						float y = whole_piece_height*(top_num_y - 1 - index_y);

						vertex.pos.x = x - Width / 2.0f;
						vertex.pos.y = y - Height / 2.0f;
						vertex.pos.z = 0.0f;
						vertex.normal.x = 0.0f;
						vertex.normal.y = 0.0f;
						vertex.normal.z = -1.0f;
						vertex.u = (x + Width*uFrameXNo) / all_width;
						vertex.v = (y + Height*uFrameYNo) / all_height;

						++target_index;
					}
				}
			}
		}

		// �o�b�t�@���A�����b�N���ď������݂��I������
		vertbuff->Unlock();


		const int c_nTotalIndexNum = index_num*frame_num;

		// ���_�C���f�b�N�X�o�b�t�@�쐬
		hr = device->CreateIndexBuffer(
			sizeof(WORD)* c_nTotalIndexNum,
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&indexbuff,
			NULL);
		if (FAILED(hr)) return false;

		// �o�b�t�@�����b�N���ď������݊J�n
		WORD * pIndex = nullptr;
		hr = indexbuff->Lock(0, 0, fw::pointer_cast<void **>(&pIndex), 0);
		if (FAILED(hr)) return false;

		/*
		* N����ɒ��_�����o�����悤�ɃC���f�b�N�X��ݒ肵�Ă䂭
		* �s�̉E�[�܂ŗ�����A�k�ރ|���S���p�ɓ����C���f�b�N�X��������ƁA
		* ���̃C���f�b�N�X��������ǉ����Ă����B
		* ������Ō�̍s�ɂȂ�܂ŌJ��Ԃ��B
		* �������Ō�̍s�����͏k�ރ|���S���p�̃C���f�b�N�X��ǉ����Ȃ��B
		*
		*/
		target_index = 0;
		for (uint uFrameIndex = 0; uFrameIndex < frame_num; ++uFrameIndex)
		{
			for (int index_y = 0; index_y < piece_num_y; ++index_y)		// �ォ�牺�̍s�Ɍ������ăC���f�b�N�X���i�[
			{
				for (int index_x = 0; index_x < top_num_x; ++index_x)	// ������E�̗�Ɍ������ăC���f�b�N�X���i�[
				{
					pIndex[target_index] = top_num_x*(index_y + 1) + index_x + index_num*uFrameIndex;	// �܂������̃C���f�b�N�X���i�[
					++target_index;
					pIndex[target_index] = top_num_x*(index_y)+index_x + index_num*uFrameIndex;			// ���ɏ㑤�̃C���f�b�N�X���i�[
					++target_index;
				}

				// �k�ރ|���S���p�̃C���f�b�N�X��ǉ�����
				const int piece_index_last = piece_num_y - 1;	// �Ō�̍s��index_y
				if (index_y < piece_index_last)	// �Ō�̍s�����O�̍s����������s
				{
					pIndex[target_index] = top_num_x*(index_y)+top_num_x - 1 + index_num*uFrameIndex;	// ���̍s�̍Ō�̃C���f�b�N�X�Ɠ����C���f�b�N�X��ǉ�
					++target_index;
					pIndex[target_index] = top_num_x*(index_y + 1 + 1) + 0 + index_num*uFrameIndex;		// ���̍s�̍ŏ��̃C���f�b�N�X�Ɠ����C���f�b�N�X��ǉ�
					++target_index;
				}
			}
		}

		// �o�b�t�@���A�����b�N���ď������ݏI��
		indexbuff->Unlock();

		return true;
	}
	bool anime_data::load(const Manager & manageri, const std::string & path, float piece_size, uint uPieceWidth, uint uPieceHeight, uint uEnableNum)
	{
		return load(manageri, path.c_str(), piece_size, uPieceWidth, uPieceHeight, uEnableNum);
	}

	uint anime_data::width() const
	{
		return Width;
	}
	uint anime_data::height() const
	{
		return Height;
	}

	uint anime_data::enable_num() const
	{
		return uEnableNum;
	}

	void anime_data::compute_world_mat(D3DXMATRIX & mat, const model_coor & coor)
	{
		D3DXMATRIX matRot, matMove;
		D3DXMatrixRotationYawPitchRoll(&matRot, coor.radian_y, coor.radian_x, coor.radian_z);
		D3DXMatrixTranslation(&matMove, coor.x, coor.y, coor.z);
		D3DXMatrixMultiply(&mat, &matRot, &matMove);
	}

	bool anime_data::draw_no_alpha(const D3DXMATRIX & world_mat, const D3DMATERIAL9 & material, uint uNo) const
	{
		// ���e�X�g�֎~
		device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

		// �������\���֎~(���u�����f�B���O������)
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);


		return draw(world_mat, material, uNo);
	}
	bool anime_data::draw_alpha(const D3DXMATRIX & world_mat, const D3DMATERIAL9 & material, int nBlendmode, uint uNo) const
	{
		// �������\�����\�ɂ���(���u�����f�B���O�L����)
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		// ���S�����������������܂Ȃ�(���e�X�g�L����)
		device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		device->SetRenderState(D3DRS_ALPHAREF, 0);
		device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		// Z�o�b�t�@���X�V���Ȃ�
		device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		// ���Z�����L����
		if (nBlendmode == BlendMode::add)
		{
			device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		}

		// ���Z�����L����
		if (nBlendmode == BlendMode::sub)
		{
			device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
			device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		}

		bool result = draw(world_mat, material, uNo);

		// ���Z��������
		if (nBlendmode == BlendMode::add)
		{
			device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}

		// ���Z��������
		if (nBlendmode == BlendMode::sub)
		{
			device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}

		// Z�o�b�t�@���X�V����
		device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		return result;
	}


	bool anime_data::draw(const D3DXMATRIX & world_mat, const D3DMATERIAL9 & material, uint uNo) const
	{
		// ���[���h�ϊ��s��ݒ�
		device->SetTransform(D3DTS_WORLD, &world_mat);

		// ���_�t�H�[�}�b�g�ݒ�
		device->SetFVF(LAND_FVF);

		//�}�e���A���ݒ�
		device->SetMaterial(&material);

		//�e�N�X�`���ݒ�
		device->SetTexture(0, texture);

		device->SetStreamSource(0, vertbuff, 0, sizeof(land_vertex));
		device->SetIndices(indexbuff);

		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		//device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		//device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		//device->SetRenderState(D3DRS_FILLMODE, D3DFILL_POINT);

		uint beg_index = index_num*uNo;

		// �v���~�e�B�u�`��
		device->DrawIndexedPrimitive(
			D3DPT_TRIANGLESTRIP,
			0,
			0,
			triangle_num*3,
			beg_index,
			triangle_num);

		return true;
	}


	void anime_data::release()
	{
		dxsaferelease(texture);
		dxsaferelease(vertbuff);
		dxsaferelease(indexbuff);
	}

	anime_data::~anime_data()
	{
		release();
	}



	void anime_data::construct()
	{
		texture = nullptr;
		vertbuff = nullptr;
		indexbuff = nullptr;
	}




}


















#if 0

#include <anime_data.h>
#include <Manager.h>
#include <utility.h>
#include <top4.h>
#include <diffuse.h>
#include <model_coor.h>

// max_vertex = 11*11
// �ȃ����O�L���i�\�̂ݕ`��j
// device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);�J�����O�L��
// device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);�J�����O����


static const WORD LAND_FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

struct land_vertex
{
	D3DVECTOR   pos;
	D3DVECTOR   normal;
	float       u, v;
};

template<class T>
void dxsaferelease(T * pointer)
{
	if (pointer != nullptr)
	{
		pointer->Release();
		pointer = nullptr;
	}
}

bool init_vertex(
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
		sizeof(land_vertex)* top_num,
		D3DUSAGE_WRITEONLY,
		LAND_FVF,
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
	int target_index = 0;
	for (int index_y = 0; index_y < top_num_y; ++index_y)		// ��̍s���牺�̍s�֒��_�����i�[���Ă䂭
	{
		for (int index_x = 0; index_x < top_num_x; ++index_x)	// ���̗񂩂�E�̗�֒��_�����i�[���Ă䂭
		{
			land_vertex & vertex = pVtx[target_index];

			vertex.pos.x = static_cast<float>(width*index_x) / piece_num_x -width/2.0f;
			vertex.pos.y = 0.0f;
			vertex.pos.z = static_cast<float>(height*(top_num_y-1-index_y)) / piece_num_y -height/2.0f;
			vertex.normal.x = 0.0f;
			vertex.normal.y = 1.0f;
			vertex.normal.z = 0.0f;
			vertex.u = static_cast<float>(index_x) / piece_num_x;
			vertex.v = static_cast<float>(index_y) / piece_num_y;

			++target_index;
		}
	}

	// �o�b�t�@���A�����b�N���ď������݂��I������
	(*vertbuff)->Unlock();

	return true;
}

bool init_index(
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

bool load(
	LPDIRECT3DDEVICE9 device,			// in
	const TCHAR * path,					// in
	LPDIRECT3DTEXTURE9 * texture,		// out
	IDirect3DVertexBuffer9 ** vertbuff,	// out
	IDirect3DIndexBuffer9 ** indexbuff,	// out
	int * index_num,					// out
	int * triangle_num,					// out
	float piece_size,					// in
	uint & width,						// out
	uint & height )					// out
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


bool draw(
	LPDIRECT3DDEVICE9 device,
	const D3DXMATRIX & world_mat,
	LPDIRECT3DTEXTURE9 texture,
	IDirect3DVertexBuffer9 * vertbuff,
	IDirect3DIndexBuffer9 * indexbuff,
	int index_num,
	int triangle_num )
{
	// ���[���h�ϊ��s��ݒ�
	device->SetTransform(D3DTS_WORLD, &world_mat);

	// ���_�t�H�[�}�b�g�ݒ�
	device->SetFVF(LAND_FVF);

	//�}�e���A���ݒ�
	static const D3DMATERIAL9 material = {
			{ 1.0f, 1.0f, 1.0f, 1.0f },		// Diffuse
			{ 0.1f, 0.1f, 0.1f, 0.1f },		// Ambient
			{ 0.0f, 0.0f, 0.0f, 0.0f },		// Specular
			{ 0.0f, 0.0f, 0.0f, 0.0f },		// Emissive
			1.0f };
	device->SetMaterial(&material);

	//�e�N�X�`���ݒ�
	device->SetTexture(0, texture);

	device->SetStreamSource(0, vertbuff, 0, sizeof(land_vertex));
	device->SetIndices(indexbuff);

	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

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
	anime_data::anime_data()
	{
		construct();
	}
	anime_data::anime_data(const Manager & manageri, const char * path, float piece_size)
	{
		construct();
		load(manageri, path, piece_size);
	}
	anime_data::anime_data(const Manager & manageri, const std::string & path, float piece_size)
	{
		construct();
		load(manageri, path, piece_size);
	}

	bool anime_data::load(const Manager & manageri, const char * path, float piece_size)
	{
#if 0
		const uint piece_size = 500;

		device = manageri.get_dxdevice();

		release();

		HRESULT hr;

		D3DXIMAGE_INFO info;
		hr = D3DXGetImageInfoFromFile(path, &info);
		if (FAILED(hr)) return false;
		Width = info.Width;
		Height = info.Height;

		int piece_num_x = Width / piece_size + 1;
		int piece_num_y = Height / piece_size + 1;
		int piece_num = piece_num_x * piece_num_y;

		int top_num_x = piece_num_x + 1;
		int top_num_y = piece_num_y + 1;
		int top_num = top_num_x*top_num_y;

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
			&texture);

		if (FAILED(hr))
		{
			return false;
		}

		hr = device->CreateVertexBuffer(
			sizeof(land_vertex)* top_num,
			D3DUSAGE_WRITEONLY,
			LAND_FVF,
			D3DPOOL_MANAGED,
			&vertbuff,
			NULL);

		if (FAILED(hr))
		{
			return false;
		}

		land_vertex * pVtx = nullptr;
		hr = vertbuff->Lock(0, 0, fw::pointer_cast<void **>(&pVtx), 0);
		if (FAILED(hr)) return false;

		int target_index = 0;
		for (int index_y = 0; index_y < top_num_y; ++index_y)
		{
			for (int index_x = 0; index_x < top_num_x; ++index_x)
			{
				land_vertex & vertex = pVtx[target_index];

				vertex.pos.x = static_cast<float>(Width*index_x) / piece_num_x;
				vertex.pos.y = 0.0f;
				vertex.pos.z = static_cast<float>(Height*index_y) / piece_num_y;
				vertex.normal.x = 0.0f;
				vertex.normal.y = 1.0f;
				vertex.normal.z = 0.0f;
				vertex.u = static_cast<float>(index_x) / piece_num_x;
				vertex.v = static_cast<float>(index_y) / piece_num_y;

				++target_index;
			}
		}

		vertbuff->Unlock();


	
		index_num = (2*top_num_x)*piece_num_y + 2*(piece_num_y-1);
		triangle_num = index_num-2;

		hr = device->CreateIndexBuffer(
			sizeof(WORD)* index_num,
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&indexbuff,
			NULL);

		if (FAILED(hr))
		{
			return false;
		}


		WORD * pIndex = nullptr;
		hr = indexbuff->Lock(0, 0, fw::pointer_cast<void **>(&pIndex), 0);
		if (FAILED(hr)) return false;

		target_index = 0;
		for (int index_y = 0; index_y < piece_num_y; ++index_y)
		{
			for (int index_x = 0; index_x < top_num_x; ++index_x)
			{
				pIndex[target_index] = top_num_x*(index_y+1) + index_x;
				++target_index;
				pIndex[target_index] = top_num_x*(index_y) + index_x;
				++target_index;
			}

			const int piece_index_last = piece_num_y - 1;
			if (index_y < piece_index_last)
			{
				pIndex[target_index] = top_num_x*(index_y) + top_num_x-1;
				++target_index;
				pIndex[target_index] = top_num_x*(index_y+1+1) + 0;
				++target_index;
			}
		}



		indexbuff->Unlock();
#endif

		device = manageri.get_dxdevice();

		::load(
			device,
			path,
			&texture,
			&vertbuff,
			&indexbuff,
			&index_num,
			&triangle_num,
			piece_size,
			Width,
			Height);

		return true;
	}
	bool anime_data::load(const Manager & manageri, const std::string & path, float piece_size)
	{
		return load(manageri, path.c_str(), piece_size);
	}

	uint anime_data::width() const
	{
		return Width;
	}
	uint anime_data::height() const
	{
		return Height;
	}

	void compute_world_mat(D3DXMATRIX & mat, const model_coor & coor)
	{
		D3DXMATRIX matRot, matMove;
		D3DXMatrixRotationYawPitchRoll(&matRot, coor.radian_y, coor.radian_x, coor.radian_z);
		D3DXMatrixTranslation(&matMove, coor.x, coor.y, coor.z);
		D3DXMatrixMultiply(&mat, &matRot, &matMove);
	}

	bool anime_data::draw(const D3DXMATRIX & world_mat) const
	{
		::draw(
			device,
			world_mat,
			texture,
			vertbuff,
			indexbuff,
			index_num,
			triangle_num);
#if 0
		// ���[���h�ϊ��s��ݒ�
		device->SetTransform(D3DTS_WORLD, &world_mat);

		// ���_�t�H�[�}�b�g�ݒ�
		device->SetFVF(LAND_FVF);

		//�}�e���A���ݒ�
		static const D3DMATERIAL9 material = {
				{ 1.0f, 1.0f, 1.0f, 1.0f },		// Diffuse
				{ 0.1f, 0.1f, 0.1f, 0.1f },		// Ambient
				{ 0.1f, 0.1f, 0.1f, 0.1f },		// Specular
				{ 0.0f, 0.0f, 0.0f, 0.0f },		// Emissive
				1.0f,							// Power
		};
		device->SetMaterial(&material);

		//�e�N�X�`���ݒ�
		device->SetTexture(0, texture);

		device->SetStreamSource(0, vertbuff, 0, sizeof(land_vertex));
		device->SetIndices(indexbuff);

		//device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		//device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		//device->SetRenderState(D3DRS_FILLMODE, D3DFILL_POINT);
		
		// �v���~�e�B�u�`��
		device->DrawIndexedPrimitive(
			D3DPT_TRIANGLESTRIP,
			0,
			0,
			9,
			0,
			7);

#endif


		return true;
	}


	void anime_data::release()
	{
		dxsaferelease(texture);
		dxsaferelease(vertbuff);
		dxsaferelease(indexbuff);
	}

	anime_data::~anime_data()
	{
		release();
	}



	void anime_data::construct()
	{
		texture = nullptr;
		vertbuff = nullptr;
		indexbuff = nullptr;
	}



#if 0
	bool anime_data::CreateRectanglePolygon(const top4 & xyz, const top4 & uv, const diffuse & rgba) const
	{
		HRESULT hr;

		vertex2D * pVtx = nullptr;
		hr = vertbuff->Lock(0, 0, fw::pointer_cast<void **>(&pVtx), 0);
		if (FAILED(hr)) return false;

		pVtx[0].vtx = D3DXVECTOR3(xyz.lt.x(), xyz.lt.y(), xyz.lt.z());
		pVtx[1].vtx = D3DXVECTOR3(xyz.rt.x(), xyz.rt.y(), xyz.rt.z());
		pVtx[2].vtx = D3DXVECTOR3(xyz.lb.x(), xyz.lb.y(), xyz.lb.z());
		pVtx[3].vtx = D3DXVECTOR3(xyz.rb.x(), xyz.rb.y(), xyz.rb.z());

#if 0
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
#endif

		pVtx[0].diffuse = D3DCOLOR_RGBA(rgba.red, rgba.green, rgba.blue, rgba.alpha);
		pVtx[1].diffuse = D3DCOLOR_RGBA(rgba.red, rgba.green, rgba.blue, rgba.alpha);
		pVtx[2].diffuse = D3DCOLOR_RGBA(rgba.red, rgba.green, rgba.blue, rgba.alpha);
		pVtx[3].diffuse = D3DCOLOR_RGBA(rgba.red, rgba.green, rgba.blue, rgba.alpha);

		pVtx[0].tex = D3DXVECTOR2(uv.lt.x(), uv.lt.y());
		pVtx[1].tex = D3DXVECTOR2(uv.rt.x(), uv.rt.y());
		pVtx[2].tex = D3DXVECTOR2(uv.lb.x(), uv.lb.y());
		pVtx[3].tex = D3DXVECTOR2(uv.rb.x(), uv.rb.y());

		vertbuff->Unlock();

		return true;
	}

	void anime_data::DrawWithoutCreatePolygon() const
	{
		D3DXMATRIX temp_mat;
		D3DXMatrixIdentity(&temp_mat);
		device->SetTransform(D3DTS_WORLD, &temp_mat);
		device->SetFVF(FVF_2D);
		device->SetStreamSource(0, vertbuff, 0, sizeof(vertex2D));
		device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
#endif

}


#endif