#if 1

#pragma once

#include "popular.h"

namespace si3
{
	class SkydomeData
	{
	public:
		SkydomeData();
		/*
		** roughness:球の曲面の粗さ。この数値が大きいほど粗くなる。
		** 1.0で最も粗くなり、長方形の板が表示される。
		** マイナス値を指定してはいけない。
		** 0.0も指定してはいけない。
		*/
		SkydomeData(LPDIRECT3DDEVICE9 device, const char * path, float roughness);
		void load(LPDIRECT3DDEVICE9 device, const char * path, float roughness);
		void draw(const D3DXMATRIX & world_mat) const;






		~SkydomeData();
	private:
		struct land_vertex
		{
			D3DVECTOR   pos;
			D3DVECTOR   normal;
			float u;
			float v;
		};

		LPDIRECT3DDEVICE9 device;
		LPDIRECT3DTEXTURE9 texture;
		IDirect3DVertexBuffer9 * vertbuff;
		IDirect3DIndexBuffer9 * indexbuff;
		int index_num;
		int triangle_num;

		bool init_vertex(
			LPDIRECT3DDEVICE9 device,			// in
			int top_num_x,						// in
			int top_num_y,						// in
			int top_num,						// in
			float width,						// in
			float height,						// in
			int piece_num_x,					// in
			int piece_num_y,					// in
			IDirect3DVertexBuffer9 ** vertbuff);// out

		bool init_index(
			LPDIRECT3DDEVICE9 device,			// in
			int index_num,						// in
			IDirect3DIndexBuffer9 ** indexbuff,	// out
			int top_num_x,						// in
			int top_num_y,						// in
			int top_num,						// in
			int piece_num_x,					// in
			int piece_num_y);					// in

		void construct();
		void release();
	};
}

#endif