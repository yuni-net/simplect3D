#include <image_data.h>
#include <Manager.h>
#include <utility.h>
#include <top4.h>
#include <diffuse.h>
#include <model_coor.h>

// max_vertex = 11*11
// 科リング有効（表のみ描画）
// device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);カリング有効
// device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);カリング無効


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

bool si3::image_data::init_vertex(
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

	// 頂点情報格納バッファを作成
	hr = device->CreateVertexBuffer(
		sizeof(land_vertex)* top_num,
		D3DUSAGE_WRITEONLY,
		LAND_FVF,
		D3DPOOL_MANAGED,
		vertbuff,
		NULL);

	if (FAILED(hr)) return false;

	// バッファをロックをして書き込みを開始する
	land_vertex * pVtx = nullptr;
	hr = (*vertbuff)->Lock(0, 0, fw::pointer_cast<void **>(&pVtx), 0);
	if (FAILED(hr)) return false;

	/*
	* 1.最も左上の頂点情報から格納してゆく
	* 2.左から右に向かって順々に頂点情報を格納してゆく
	* 3.一番右まで格納し終えたら、一つ下の行に移動する
	* 4.2→3を、最後の行になるまで繰り返す
	*
	*/
	int target_index = 0;
	for (int index_y = 0; index_y < top_num_y; ++index_y)		// 上の行から下の行へ頂点情報を格納してゆく
	{
		for (int index_x = 0; index_x < top_num_x; ++index_x)	// 左の列から右の列へ頂点情報を格納してゆく
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

	// バッファをアンロックして書き込みを終了する
	(*vertbuff)->Unlock();

	return true;
}

bool si3::image_data::init_index(
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

	// 頂点インデックスバッファ作成
	hr = device->CreateIndexBuffer(
		sizeof(WORD)* index_num,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		indexbuff,
		NULL);
	if (FAILED(hr)) return false;

	// バッファをロックして書き込み開始
	WORD * pIndex = nullptr;
	hr = (*indexbuff)->Lock(0, 0, fw::pointer_cast<void **>(&pIndex), 0);
	if (FAILED(hr)) return false;

	/*
	* N字状に頂点が取り出されるようにインデックスを設定してゆく
	* 行の右端まで来たら、縮退ポリゴン用に同じインデックスをもう一個と、
	* 次のインデックスをもう一個追加しておく。
	* これを最後の行になるまで繰り返す。
	* ただし最後の行だけは縮退ポリゴン用のインデックスを追加しない。
	*
	*/
	int target_index = 0;
	for (int index_y = 0; index_y < piece_num_y; ++index_y)		// 上から下の行に向かってインデックスを格納
	{
		for (int index_x = 0; index_x < top_num_x; ++index_x)	// 左から右の列に向かってインデックスを格納
		{
			pIndex[target_index] = top_num_x*(index_y + 1) + index_x;	// まず下側のインデックスを格納
			++target_index;
			pIndex[target_index] = top_num_x*(index_y)+index_x;			// 次に上側のインデックスを格納
			++target_index;
		}

		// 縮退ポリゴン用のインデックスを追加する
		const int piece_index_last = piece_num_y - 1;	// 最後の行のindex_y
		if (index_y < piece_index_last)	// 最後の行よりも前の行だったら実行
		{
			pIndex[target_index] = top_num_x*(index_y)+top_num_x - 1;	// この行の最後のインデックスと同じインデックスを追加
			++target_index;
			pIndex[target_index] = top_num_x*(index_y + 1 + 1) + 0;		// 次の行の最初のインデックスと同じインデックスを追加
			++target_index;
		}
	}

	// バッファをアンロックして書き込み終了
	(*indexbuff)->Unlock();

	return true;
}

bool si3::image_data::load(
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

	// テクスチャのサイズを調べて記憶する
	D3DXIMAGE_INFO info;
	hr = D3DXGetImageInfoFromFile(path, &info);
	if (FAILED(hr)) return false;
	width = info.Width;
	height = info.Height;

	int piece_num_x = static_cast<int>(width / piece_size + 1);	// 板ポリゴンを格子状に分割管理したときの列数
	int piece_num_y = static_cast<int>(height / piece_size + 1);	// 板ポリゴンを格子状に分割管理したときの行数
	int piece_num = piece_num_x * piece_num_y;	// 板ポリゴンを格子状に分割管理したときの断片の個数

	int top_num_x = piece_num_x + 1;			// 板ポリゴンを格子状に分割管理したときの頂点の列数
	int top_num_y = piece_num_y + 1;			// 板ポリゴンを格子状に分割管理したときの頂点の行数
	int top_num = top_num_x*top_num_y;			// 板ポリゴンを格子状に分割管理したときの頂点の個数

	const int top_per_row = 2 * top_num_x;				// 一行につき何個の頂点情報を使用するか
	const int top_of_grid = top_per_row * piece_num_y;	// 格子状の頂点情報の合計
	const int top_for_degen_per_row = 2;				// 一行につき必要な縮退ポリゴン用の頂点情報の数
	const int top_for_degen = top_for_degen_per_row * (piece_num_y - 1);// 縮退ポリゴン用の頂点情報の合計
	*index_num = top_of_grid + top_for_degen;			// 頂点インデックス情報の個数
	*triangle_num = *index_num - 2;						// 三角ポリゴンの合計

	// テクスチャ作成
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

	// 頂点バッファ作成、頂点データ設定
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

	// 頂点インデックスバッファ作成、頂点インデックスデータ設定
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


bool si3::image_data::draw(
	LPDIRECT3DDEVICE9 device,
	const D3DXMATRIX & world_mat,
	LPDIRECT3DTEXTURE9 texture,
	IDirect3DVertexBuffer9 * vertbuff,
	IDirect3DIndexBuffer9 * indexbuff,
	int index_num,
	int triangle_num,
	const D3DMATERIAL9 & material) const
{
	// ワールド変換行列設定
	device->SetTransform(D3DTS_WORLD, &world_mat);

	// 頂点フォーマット設定
	device->SetFVF(LAND_FVF);

	//マテリアル設定
	device->SetMaterial(&material);

	//テクスチャ設定
	device->SetTexture(0, texture);

	device->SetStreamSource(0, vertbuff, 0, sizeof(land_vertex));
	device->SetIndices(indexbuff);


	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	//device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//device->SetRenderState(D3DRS_FILLMODE, D3DFILL_POINT);

	// プリミティブ描画
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
	image_data::image_data()
	{
		construct();
	}
	image_data::image_data(const Manager & manageri, const char * path, float piece_size)
	{
		construct();
		load(manageri, path, piece_size);
	}
	image_data::image_data(const Manager & manageri, const std::string & path, float piece_size)
	{
		construct();
		load(manageri, path, piece_size);
	}

	bool image_data::load(const Manager & manageri, const char * path, float piece_size)
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
			Width,
			Height);

		return true;
	}
	bool image_data::load(const Manager & manageri, const std::string & path, float piece_size)
	{
		return load(manageri, path.c_str(), piece_size);
	}

	uint image_data::width() const
	{
		return Width;
	}
	uint image_data::height() const
	{
		return Height;
	}

	void image_data::compute_world_mat(D3DXMATRIX & mat, const model_coor & coor)
	{
		D3DXMATRIX matRot, matMove;
		D3DXMatrixRotationYawPitchRoll(&matRot, coor.radian_y, coor.radian_x, coor.radian_z);
		D3DXMatrixTranslation(&matMove, coor.x, coor.y, coor.z);
		D3DXMatrixMultiply(&mat, &matRot, &matMove);
	}

	bool image_data::draw_no_alpha(const D3DXMATRIX & world_mat, const D3DMATERIAL9 & material, bool bCulling) const
	{
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

		if (bCulling)
		{
			device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		}
		else
		{
			device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		}

		return draw(world_mat, material);
	}
	bool image_data::draw_alpha(const D3DXMATRIX & world_mat, const D3DMATERIAL9 & material, int blendmode, bool bCulling) const
	{
		// 半透明表示を可能にする(αブレンディング有効化)
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		// 完全透明部分を書き込まない(αテスト有効化)
		device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		device->SetRenderState(D3DRS_ALPHAREF, 0);
		device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		// マテリアルとテクスチャのrgba情報を掛け合わせる設定
		device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
		device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);

		if (bCulling)
		{
			device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		}
		else
		{
			device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		}

		// Zバッファを更新しない
		device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		// 加算合成有効化
		if (blendmode == add_blend)
		{
			device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		}

		// 減算合成有効化
		if (blendmode == sub_blend)
		{
			device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
			device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		}

		bool result = draw(world_mat, material);

		// 加算合成解除
		if (blendmode == add_blend)
		{
			device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}

		// 減算合成解除
		if (blendmode == sub_blend)
		{
			device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}

		// Zバッファを更新する
		device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		return result;
	}




	bool image_data::draw(const D3DXMATRIX & world_mat, const D3DMATERIAL9 & material) const
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


	void image_data::release()
	{
		dxsaferelease(texture);
		dxsaferelease(vertbuff);
		dxsaferelease(indexbuff);
	}

	image_data::~image_data()
	{
		release();
	}



	void image_data::construct()
	{
		texture = nullptr;
		vertbuff = nullptr;
		indexbuff = nullptr;
	}




}


















#if 0

#include <image_data.h>
#include <Manager.h>
#include <utility.h>
#include <top4.h>
#include <diffuse.h>
#include <model_coor.h>

// max_vertex = 11*11
// 科リング有効（表のみ描画）
// device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);カリング有効
// device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);カリング無効


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

	// 頂点情報格納バッファを作成
	hr = device->CreateVertexBuffer(
		sizeof(land_vertex)* top_num,
		D3DUSAGE_WRITEONLY,
		LAND_FVF,
		D3DPOOL_MANAGED,
		vertbuff,
		NULL);

	if (FAILED(hr)) return false;
 
	// バッファをロックをして書き込みを開始する
	land_vertex * pVtx = nullptr;
	hr = (*vertbuff)->Lock(0, 0, fw::pointer_cast<void **>(&pVtx), 0);
	if (FAILED(hr)) return false;

	/*
	 * 1.最も左上の頂点情報から格納してゆく
	 * 2.左から右に向かって順々に頂点情報を格納してゆく
	 * 3.一番右まで格納し終えたら、一つ下の行に移動する
	 * 4.2→3を、最後の行になるまで繰り返す
	 *
	 */
	int target_index = 0;
	for (int index_y = 0; index_y < top_num_y; ++index_y)		// 上の行から下の行へ頂点情報を格納してゆく
	{
		for (int index_x = 0; index_x < top_num_x; ++index_x)	// 左の列から右の列へ頂点情報を格納してゆく
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

	// バッファをアンロックして書き込みを終了する
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

	// 頂点インデックスバッファ作成
	hr = device->CreateIndexBuffer(
		sizeof(WORD)* index_num,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		indexbuff,
		NULL);
	if (FAILED(hr)) return false;

	// バッファをロックして書き込み開始
	WORD * pIndex = nullptr;
	hr = (*indexbuff)->Lock(0, 0, fw::pointer_cast<void **>(&pIndex), 0);
	if (FAILED(hr)) return false;

	/*
	 * N字状に頂点が取り出されるようにインデックスを設定してゆく
	 * 行の右端まで来たら、縮退ポリゴン用に同じインデックスをもう一個と、
	 * 次のインデックスをもう一個追加しておく。
	 * これを最後の行になるまで繰り返す。
	 * ただし最後の行だけは縮退ポリゴン用のインデックスを追加しない。
	 *
	 */
	int target_index = 0;
	for (int index_y = 0; index_y < piece_num_y; ++index_y)		// 上から下の行に向かってインデックスを格納
	{
		for (int index_x = 0; index_x < top_num_x; ++index_x)	// 左から右の列に向かってインデックスを格納
		{
			pIndex[target_index] = top_num_x*(index_y + 1) + index_x;	// まず下側のインデックスを格納
			++target_index;
			pIndex[target_index] = top_num_x*(index_y)+index_x;			// 次に上側のインデックスを格納
			++target_index;
		}

		// 縮退ポリゴン用のインデックスを追加する
		const int piece_index_last = piece_num_y - 1;	// 最後の行のindex_y
		if (index_y < piece_index_last)	// 最後の行よりも前の行だったら実行
		{
			pIndex[target_index] = top_num_x*(index_y)+top_num_x - 1;	// この行の最後のインデックスと同じインデックスを追加
			++target_index;
			pIndex[target_index] = top_num_x*(index_y + 1 + 1) + 0;		// 次の行の最初のインデックスと同じインデックスを追加
			++target_index;
		}
	}

	// バッファをアンロックして書き込み終了
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

	// テクスチャのサイズを調べて記憶する
	D3DXIMAGE_INFO info;
	hr = D3DXGetImageInfoFromFile(path, &info);
	if (FAILED(hr)) return false;
	width = info.Width;
	height = info.Height;

	int piece_num_x = static_cast<int>(width / piece_size + 1);	// 板ポリゴンを格子状に分割管理したときの列数
	int piece_num_y = static_cast<int>(height / piece_size + 1);	// 板ポリゴンを格子状に分割管理したときの行数
	int piece_num = piece_num_x * piece_num_y;	// 板ポリゴンを格子状に分割管理したときの断片の個数

	int top_num_x = piece_num_x + 1;			// 板ポリゴンを格子状に分割管理したときの頂点の列数
	int top_num_y = piece_num_y + 1;			// 板ポリゴンを格子状に分割管理したときの頂点の行数
	int top_num = top_num_x*top_num_y;			// 板ポリゴンを格子状に分割管理したときの頂点の個数

	const int top_per_row = 2 * top_num_x;				// 一行につき何個の頂点情報を使用するか
	const int top_of_grid = top_per_row * piece_num_y;	// 格子状の頂点情報の合計
	const int top_for_degen_per_row = 2;				// 一行につき必要な縮退ポリゴン用の頂点情報の数
	const int top_for_degen = top_for_degen_per_row * (piece_num_y - 1);// 縮退ポリゴン用の頂点情報の合計
	*index_num = top_of_grid + top_for_degen;			// 頂点インデックス情報の個数
	*triangle_num = *index_num - 2;						// 三角ポリゴンの合計

	// テクスチャ作成
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

	// 頂点バッファ作成、頂点データ設定
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

	// 頂点インデックスバッファ作成、頂点インデックスデータ設定
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
	// ワールド変換行列設定
	device->SetTransform(D3DTS_WORLD, &world_mat);

	// 頂点フォーマット設定
	device->SetFVF(LAND_FVF);

	//マテリアル設定
	static const D3DMATERIAL9 material = {
			{ 1.0f, 1.0f, 1.0f, 1.0f },		// Diffuse
			{ 0.1f, 0.1f, 0.1f, 0.1f },		// Ambient
			{ 0.0f, 0.0f, 0.0f, 0.0f },		// Specular
			{ 0.0f, 0.0f, 0.0f, 0.0f },		// Emissive
			1.0f };
	device->SetMaterial(&material);

	//テクスチャ設定
	device->SetTexture(0, texture);

	device->SetStreamSource(0, vertbuff, 0, sizeof(land_vertex));
	device->SetIndices(indexbuff);

	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	//device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//device->SetRenderState(D3DRS_FILLMODE, D3DFILL_POINT);

	// プリミティブ描画
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
	image_data::image_data()
	{
		construct();
	}
	image_data::image_data(const Manager & manageri, const char * path, float piece_size)
	{
		construct();
		load(manageri, path, piece_size);
	}
	image_data::image_data(const Manager & manageri, const std::string & path, float piece_size)
	{
		construct();
		load(manageri, path, piece_size);
	}

	bool image_data::load(const Manager & manageri, const char * path, float piece_size)
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
	bool image_data::load(const Manager & manageri, const std::string & path, float piece_size)
	{
		return load(manageri, path.c_str(), piece_size);
	}

	uint image_data::width() const
	{
		return Width;
	}
	uint image_data::height() const
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

	bool image_data::draw(const D3DXMATRIX & world_mat) const
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
		// ワールド変換行列設定
		device->SetTransform(D3DTS_WORLD, &world_mat);

		// 頂点フォーマット設定
		device->SetFVF(LAND_FVF);

		//マテリアル設定
		static const D3DMATERIAL9 material = {
				{ 1.0f, 1.0f, 1.0f, 1.0f },		// Diffuse
				{ 0.1f, 0.1f, 0.1f, 0.1f },		// Ambient
				{ 0.1f, 0.1f, 0.1f, 0.1f },		// Specular
				{ 0.0f, 0.0f, 0.0f, 0.0f },		// Emissive
				1.0f,							// Power
		};
		device->SetMaterial(&material);

		//テクスチャ設定
		device->SetTexture(0, texture);

		device->SetStreamSource(0, vertbuff, 0, sizeof(land_vertex));
		device->SetIndices(indexbuff);

		//device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		//device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		//device->SetRenderState(D3DRS_FILLMODE, D3DFILL_POINT);
		
		// プリミティブ描画
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


	void image_data::release()
	{
		::si3::dxsaferelease(texture);
		::si3::dxsaferelease(vertbuff);
		::si3::dxsaferelease(indexbuff);
	}

	image_data::~image_data()
	{
		release();
	}



	void image_data::construct()
	{
		texture = nullptr;
		vertbuff = nullptr;
		indexbuff = nullptr;
	}



#if 0
	bool image_data::CreateRectanglePolygon(const top4 & xyz, const top4 & uv, const diffuse & rgba) const
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

	void image_data::DrawWithoutCreatePolygon() const
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