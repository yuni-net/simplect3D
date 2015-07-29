#if 1

#pragma once

#include "si3_popular.h"

namespace si3
{
	class SphereData
	{
	public:
		SphereData();

		/*
		** smoothness:球の曲面のなめらかさ。この数値が大きいほど滑らかになる。
		** 1.0で最も滑らかくなる。
		** マイナス値を指定してはいけない。
		*/
		SphereData(const char * path, float smoothness, float radius);
		void load(const char * path, float smoothness, float radius);
		void draw(const D3DXMATRIX & world_mat) const;






		~SphereData();
	private:

		LPDIRECT3DDEVICE9 device;
		LPDIRECT3DTEXTURE9 texture;
		LPD3DXMESH mesh;


		void construct();
		void release();
	};
}

#endif