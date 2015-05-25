#if 1

#pragma once

#include "popular.h"

namespace si3
{
	class sphere_data
	{
	public:
		sphere_data();
		/*
		** roughness:球の曲面の粗さ。この数値が大きいほど粗くなる。
		** 1.0で最も粗くなり、長方形の板が表示される。
		** マイナス値を指定してはいけない。
		** 0.0も指定してはいけない。
		*/
		sphere_data(LPDIRECT3DDEVICE9 device, const char * path, float smoothness, float radius);
		void load(LPDIRECT3DDEVICE9 device, const char * path, float smoothness, float radius);
		void draw(const D3DXMATRIX & world_mat) const;






		~sphere_data();
	private:

		LPDIRECT3DDEVICE9 device;
		LPDIRECT3DTEXTURE9 texture;
		LPD3DXMESH mesh;


		void construct();
		void release();
	};
}

#endif