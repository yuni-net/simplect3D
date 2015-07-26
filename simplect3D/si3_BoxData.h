#if 1

#pragma once

#include "si3_popular.h"
#include "si3_Matrix.h"

namespace si3
{
	class BoxData
	{
	public:
		BoxData();
		BoxData(LPDIRECT3DDEVICE9 device, float size_x, float size_y, float size_z);
		void create(LPDIRECT3DDEVICE9 device, float size_x, float size_y, float size_z);
		void draw() const;


		LPDIRECT3DDEVICE9 get_device() const;


		~BoxData();

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
			const Matrix & scale_mat,
			const Matrix & rot_mat,
			land_vertex * vertex,
			int face_No);
	};
}

#endif