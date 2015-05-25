#if 1

#pragma once

#include "popular.h"
#include "matrix.h"

namespace si3
{
	class box_data
	{
	public:
		box_data();
		box_data(LPDIRECT3DDEVICE9 device, float size_x, float size_y, float size_z);
		void create(LPDIRECT3DDEVICE9 device, float size_x, float size_y, float size_z);
		void draw() const;


		LPDIRECT3DDEVICE9 get_device() const;


		~box_data();

	private:
		struct land_vertex
		{
			D3DVECTOR   pos;
			D3DVECTOR   normal;
		//	float       u, v;
		};

		LPDIRECT3DDEVICE9 device;
	//	LPDIRECT3DTEXTURE9 texture;
		IDirect3DVertexBuffer9 * vertbuff;
		IDirect3DIndexBuffer9 * indexbuff;
		int top_num;
		int triangle_num;

		bool init_vertex(float size_x, float size_y, float size_z);

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

		void set(
			land_vertex base[4],
			const matrix & scale_mat,
			const matrix & rot_mat,
			land_vertex * pVtx,
			int face_No);
	};
}

#endif