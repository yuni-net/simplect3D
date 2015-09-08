#if 1

#include <si3_utility.h>
#include <si3_BoxData.h>
#include <si3_Matrix.h>
#include <si3_Manager.h>

//static const WORD LAND_FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
static const WORD LAND_FVF = D3DFVF_XYZ | D3DFVF_NORMAL;

namespace si3
{
	BoxData::BoxData()
	{
		construct();
	}

	BoxData::BoxData(float size_x, float size_y, float size_z)
	{
		construct();
		create(size_x, size_y, size_z);
	}

	void BoxData::create(float size_x, float size_y, float size_z)
	{
		this->device = si3::Manager::get_dxdevice();

		release();

		bool result;

		// 頂点バッファ作成、頂点データ設定
		result = init_vertex(size_x,size_y,size_z);
		if (result == false) return;
	}

	bool BoxData::init_vertex(float size_x, float size_y, float size_z)
	{
		HRESULT	hr;

		top_num = 6 * 6 -1;	// +5は縮退ポリゴン用
		triangle_num = top_num - 2;

		// 頂点情報格納バッファを作成
		hr = device->CreateVertexBuffer(
			sizeof(land_vertex)* top_num,
			D3DUSAGE_WRITEONLY,
			LAND_FVF,
			D3DPOOL_MANAGED,
			&vertbuff,
			NULL);

		if (FAILED(hr)) return false;

		// バッファをロックをして書き込みを開始する
		land_vertex * vertex = nullptr;
		hr = (*vertbuff).Lock(0, 0, fw::pointer_cast<void **>(&vertex), 0);
		if (FAILED(hr)) return false;

		land_vertex base[4];

		for (int base_No = 0; base_No < 4; ++base_No)
		{
			base[base_No].normal.x = 0.0f;
			base[base_No].normal.y = 0.0f;
			base[base_No].normal.z = -1.0f;
		}

		int basedex = 0;

		base[basedex].pos.x = -0.5f;
		base[basedex].pos.y = -0.5f;
		base[basedex].pos.z = -0.5f;
		++basedex;

		base[basedex].pos.x = -0.5f;
		base[basedex].pos.y = 0.5f;
		base[basedex].pos.z = -0.5f;
		++basedex;

		base[basedex].pos.x = 0.5f;
		base[basedex].pos.y = -0.5f;
		base[basedex].pos.z = -0.5f;
		++basedex;

		base[basedex].pos.x = 0.5f;
		base[basedex].pos.y = 0.5f;
		base[basedex].pos.z = -0.5f;

		Matrix scale_mat;
		scale_mat.scale(size_x, size_y, size_z);

		set(base, scale_mat, Matrix().rotate_y(0.0f), vertex, 0);
		set(base, scale_mat, Matrix().rotate_y(pi*0.5f), vertex, 1);
		set(base, scale_mat, Matrix().rotate_y(pi), vertex, 2);
		set(base, scale_mat, Matrix().rotate_y(pi*1.5f), vertex, 3);
		set(base, scale_mat, Matrix().rotate_x(-pi*0.5f), vertex, 4);
		set(base, scale_mat, Matrix().rotate_x(pi*0.5f), vertex, 5);


		// バッファをアンロックして書き込みを終了する
		(*vertbuff).Unlock();

		return true;
	}

	void BoxData::set(
		land_vertex base[4],
		const Matrix & scale_mat,
		const Matrix & rot_mat,
		land_vertex * vertex,
		int face_No)
	{

		for (int top_No = 0; top_No < 4; ++top_No)
		{
			const land_vertex & abase = base[top_No];
			land_vertex & target = vertex[6 * face_No + 1 + top_No];

			Matrix normal_mat;
			normal_mat.x(abase.normal.x);
			normal_mat.y(abase.normal.y);
			normal_mat.z(abase.normal.z);

			normal_mat *= rot_mat;

			target.normal.x = normal_mat.x();
			target.normal.y = normal_mat.y();
			target.normal.z = normal_mat.z();

			Matrix trans_mat = rot_mat*scale_mat;

			Matrix pos_mat;
			pos_mat.x(abase.pos.x);
			pos_mat.y(abase.pos.y);
			pos_mat.z(abase.pos.z);

			pos_mat *= trans_mat;

			target.pos.x = pos_mat.x();
			target.pos.y = pos_mat.y();
			target.pos.z = pos_mat.z();
		}

		if (face_No > 0)
		{
			int index = 6 * face_No + 1 + 0;
			int preindex = index - 1;
			vertex[preindex] = vertex[index];
		}

		if (face_No < 5)
		{
			int index = 6 * face_No + 1 + 3;
			int nextindex = index + 1;
			vertex[nextindex] = vertex[index];
		}
	}


	void BoxData::draw() const
	{
		// 頂点フォーマット設定
		device->SetFVF(LAND_FVF);
		device->SetStreamSource(0, vertbuff, 0, sizeof(land_vertex));

		device->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,
			0,
			triangle_num);
	}


	LPDIRECT3DDEVICE9 BoxData::get_device() const
	{
		return device;
	}



	void BoxData::construct()
	{
		device = nullptr;
		vertbuff = nullptr;
		indexbuff = nullptr;
	}

	void BoxData::release()
	{
		dxsaferelease(vertbuff);
		dxsaferelease(indexbuff);
	}

	BoxData::~BoxData()
	{
		release();
	}
}

#endif