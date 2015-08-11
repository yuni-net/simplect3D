#ifndef si3_SealData_h_
#define si3_SealData_h_

#include <string>
#include "si3_popular.h"
#include "si3_Top4.h"
#include "si3_ModelCoor.h"
#include "si3_Diffuse.h"

namespace si3
{
	class Manager;

	class SealData
	{
	public:
		enum
		{
			normal_blend,
			alpha_blend,
			add_blend,
			sub_blend
		};

		SealData();
		SealData(const char * path, float piece_size);
		SealData(const std::string & path, float piece_size);

		bool load(const char * path, float piece_size);
		bool load(const std::string & path, float piece_size);

		uint width() const;
		uint height() const;

		bool draw_no_alpha(const D3DXMATRIX & world_mat, const D3DMATERIAL9 & material) const;
		bool draw_alpha(const D3DXMATRIX & world_mat, const D3DMATERIAL9 & material, int blend_mode) const;




		~SealData();


	protected:

		uint width_;
		uint height_;




		LPDIRECT3DTEXTURE9 texture;
		LPDIRECT3DDEVICE9 device;
		IDirect3DVertexBuffer9 * vertbuff;
		IDirect3DIndexBuffer9 * indexbuff;
		int index_num;
		int triangle_num;

		void construct();
		void release();

		bool draw(const D3DXMATRIX & world_mat, const D3DMATERIAL9 & material) const;

		bool CreateRectanglePolygon(const Top4 & xyz, const Top4 & uv, const Diffuse & rgba) const;
		void DrawWithoutCreatePolygon() const;
		void compute_world_mat(D3DXMATRIX & mat, const ModelCoor & coor);

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

#endif