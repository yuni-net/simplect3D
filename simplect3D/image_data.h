#pragma once

#include "popular.h"
#include "top4.h"
#include "diffuse.h"
#include "model_coor.h"

namespace si3
{
	class Manager;

	class image_data
	{
	public:
		enum
		{
			normal_blend,
			alpha_blend,
			add_blend,
			sub_blend
		};

		image_data();
		image_data(const Manager & manageri, const char * path, float piece_size);
		image_data(const Manager & manageri, const std::string & path, float piece_size);

		bool load(const Manager & manageri, const char * path, float piece_size);
		bool load(const Manager & manageri, const std::string & path, float piece_size);

		uint width() const;
		uint height() const;

		bool draw_no_alpha(const D3DXMATRIX & world_mat, const D3DMATERIAL9 & material, bool bCulling) const;
		bool draw_alpha(const D3DXMATRIX & world_mat, const D3DMATERIAL9 & material, int BlendMode, bool bCulling) const;




		~image_data();


	protected:

		uint Width;
		uint Height;



		struct land_vertex
		{
			D3DVECTOR   pos;
			D3DVECTOR   normal;
			float       u, v;
		};

		LPDIRECT3DTEXTURE9 texture;
		LPDIRECT3DDEVICE9 device;
		IDirect3DVertexBuffer9 * vertbuff;
		IDirect3DIndexBuffer9 * indexbuff;
		int index_num;
		int triangle_num;

		void construct();
		void release();

		bool draw(const D3DXMATRIX & world_mat, const D3DMATERIAL9 & material) const;

		bool CreateRectanglePolygon(const top4 & xyz, const top4 & uv, const diffuse & rgba) const;
		void DrawWithoutCreatePolygon() const;
		void compute_world_mat(D3DXMATRIX & mat, const model_coor & coor);

		bool init_vertex(
			LPDIRECT3DDEVICE9 device,			// in
			int top_num_x,						// in
			int top_num_y,						// in
			int top_num,						// in
			int width,							// in
			int height,							// in
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
			uint & height);					// out

		bool draw(
			LPDIRECT3DDEVICE9 device,
			const D3DXMATRIX & world_mat,
			LPDIRECT3DTEXTURE9 texture,
			IDirect3DVertexBuffer9 * vertbuff,
			IDirect3DIndexBuffer9 * indexbuff,
			int index_num,
			int triangle_num,
			const D3DMATERIAL9 & material) const;
	};
}