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
		** roughness:���̋Ȗʂ̑e���B���̐��l���傫���قǑe���Ȃ�B
		** 1.0�ōł��e���Ȃ�A�����`�̔��\�������B
		** �}�C�i�X�l���w�肵�Ă͂����Ȃ��B
		** 0.0���w�肵�Ă͂����Ȃ��B
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