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
		** smoothness:���̋Ȗʂ̂Ȃ߂炩���B���̐��l���傫���قǊ��炩�ɂȂ�B
		** 1.0�ōł����炩���Ȃ�B
		** �}�C�i�X�l���w�肵�Ă͂����Ȃ��B
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