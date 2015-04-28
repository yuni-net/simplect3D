#if 1

#include <utility.h>
#include <sphere_data.h>

static const WORD SPHERE_FVF = D3DFVF_XYZ | D3DFVF_NORMAL;
//static const WORD SPHERE_FVF = D3DFVF_XYZ;

namespace si3
{
	sphere_data::sphere_data(LPDIRECT3DDEVICE9 device, float roughness)
	{
		this->device = device;

		saferelease(vertbuff);
		saferelease(indexbuff);

#if 0
		HRESULT hr;

		// テクスチャのサイズを調べて記憶する
		D3DXIMAGE_INFO info;
		hr = D3DXGetImageInfoFromFile(path, &info);
		if (FAILED(hr)) return false;
		width = info.Width;
		height = info.Height;
#endif
		const float diameter = 2.0f;
		const float width = diameter * pi;
		const float height = diameter;

		int piece_num_x = static_cast<int>(width / roughness) + 1;	// 板ポリゴンを格子状に分割管理したときの列数
		int piece_num_y = static_cast<int>(height / roughness) + 1;	// 板ポリゴンを格子状に分割管理したときの行数
		int piece_num = piece_num_x * piece_num_y;	// 板ポリゴンを格子状に分割管理したときの断片の個数

		int top_num_x = piece_num_x + 1;			// 板ポリゴンを格子状に分割管理したときの頂点の列数
		int top_num_y = piece_num_y + 1;			// 板ポリゴンを格子状に分割管理したときの頂点の行数
		int top_num = top_num_x*top_num_y;			// 板ポリゴンを格子状に分割管理したときの頂点の個数

		const int top_per_row = 2 * top_num_x;				// 一行につき何個の頂点情報を使用するか
		const int top_of_grid = top_per_row * piece_num_y;	// 格子状の頂点情報の合計
		const int top_for_degen_per_row = 2;				// 一行につき必要な縮退ポリゴン用の頂点情報の数
		const int top_for_degen = top_for_degen_per_row * (piece_num_y - 1);// 縮退ポリゴン用の頂点情報の合計
		index_num = top_of_grid + top_for_degen;			// 頂点インデックス情報の個数
		triangle_num = index_num - 2;						// 三角ポリゴンの合計

#if 0
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
#endif

		bool result;

		// 頂点バッファ作成、頂点データ設定
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

		// 頂点インデックスバッファ作成、頂点インデックスデータ設定
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

	bool sphere_data::init_vertex(
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

		// 頂点情報格納バッファを作成
		hr = device->CreateVertexBuffer(
			sizeof(land_vertex)* top_num,
			D3DUSAGE_WRITEONLY,
			SPHERE_FVF,
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

				float theta = pi*index_y / piece_num_y;
				vertex.pos.y = height*cos(theta) / 2.0f;

				float radian = 2*pi * index_x/piece_num_x;
				float radius = sin(theta);

				vertex.pos.x = cos(radian)*radius;
				vertex.pos.z = sin(radian)*radius;

				// 法線ベクトルは原点からxyzまでのベクトルを単位ベクトルに直したものと一致する
				// 内部で単位ベクトルを計算し、vertex.normalに代入している
				vector_to_unit_vector(vertex.pos, vertex.normal);

				++target_index;
			}
		}

		// バッファをアンロックして書き込みを終了する
		(*vertbuff)->Unlock();

		return true;
	}

	bool sphere_data::init_index(
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

	void sphere_data::draw(const D3DXMATRIX & world_mat) const
	{
		// ワールド変換行列設定
		device->SetTransform(D3DTS_WORLD, &world_mat);

		// 頂点フォーマット設定
		device->SetFVF(SPHERE_FVF);

		//マテリアル設定
		static const D3DMATERIAL9 material = {
				{ 1.0f, 1.0f, 1.0f, 1.0f },		// Diffuse
				{ 0.9f, 0.9f, 0.9f, 0.9f },		// Ambient
				{ 0.1f, 0.1f, 0.1f, 0.1f },		// Specular
				{ 0.0f, 0.0f, 0.0f, 0.0f },		// Emissive
				1.0f };
		device->SetMaterial(&material);

		device->SetStreamSource(0, vertbuff, 0, sizeof(land_vertex));
		device->SetIndices(indexbuff);

		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		//device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		//device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		//device->SetRenderState(D3DRS_FILLMODE, D3DFILL_POINT);

		// プリミティブ描画
		device->DrawIndexedPrimitive(
			D3DPT_TRIANGLESTRIP,
			0,
			0,
			index_num,
			0,
			triangle_num);

	}


}

#endif