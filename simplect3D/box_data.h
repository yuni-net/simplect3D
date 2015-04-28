#if 0

#pragma once

#include "popular.h"

namespace si3
{
	class box_data
	{
	public:
		box_data(LPDIRECT3DDEVICE9 device, float roughness);
		void draw() const;







	private:
		struct land_vertex
		{
			D3DVECTOR   pos;
			D3DVECTOR   normal;
			float       u, v;
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

	};
}

#endif