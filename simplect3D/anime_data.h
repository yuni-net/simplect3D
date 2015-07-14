#pragma once

#include "popular.h"
#include "top4.h"
#include "diffuse.h"
#include "model_coor.h"

namespace si3
{
	class Manager;

	class anime_data
	{
	public:

		anime_data();
		anime_data(const Manager & manageri, const char * path, float piece_size, uint uPieceWidth, uint uPieceHeight, uint uEnableNum);
		anime_data(const Manager & manageri, const std::string & path, float piece_size, uint uPieceWidth, uint uPieceHeight, uint uEnableNum);

		bool load(const Manager & manageri, const char * path, float piece_size, uint uPieceWidth, uint uPieceHeight, uint uEnableNum);
		bool load(const Manager & manageri, const std::string & path, float piece_size, uint uPieceWidth, uint uPieceHeight, uint uEnableNum);

		uint width() const;
		uint height() const;
		uint enable_num() const;

		bool draw_no_alpha(const D3DXMATRIX & world_mat, const D3DMATERIAL9 & material, uint uNo) const;
		bool draw_alpha(const D3DXMATRIX & world_mat, const D3DMATERIAL9 & material, int BlendMode, uint uNo) const;





		~anime_data();


	protected:

		uint Width;
		uint Height;
		uint uEnableNum;



		struct land_vertex
		{
			D3DVECTOR   pos;
			D3DVECTOR   normal;
			float       u, v;
		};

		class grid
		{
		public:
			grid(uint uWidth, uint uHeight, float piece_size);
			int piece_num_x() const;
			int piece_num_y() const;
			int piece_num() const;
			int top_num_x() const;
			int top_num_y() const;
			int top_num() const;
			int index_num() const;
			int triangle_num() const;

		private:
			int piece_num_x_;
			int piece_num_y_;
			int piece_num_;
			int top_num_x_;
			int top_num_y_;
			int top_num_;
			int index_num_;
			int triangle_num_;
		};

		LPDIRECT3DTEXTURE9 texture;
		LPDIRECT3DDEVICE9 device;
		IDirect3DVertexBuffer9 * vertbuff;
		IDirect3DIndexBuffer9 * indexbuff;
		int index_num;
		int triangle_num;
		int piece_top_num;
		int index_num_all;

		void construct();
		void release();

		bool draw(const D3DXMATRIX & world_mat, const D3DMATERIAL9 & material, uint uNo) const;

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