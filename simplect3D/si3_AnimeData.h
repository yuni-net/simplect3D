#pragma once

#include "si3_popular.h"
#include "si3_Top4.h"
#include "si3_Diffuse.h"
#include "si3_ModelCoor.h"

namespace si3
{
	class Manager;

	class AnimeData
	{
	public:

		AnimeData();
		AnimeData(const Manager & manageri, const char * path, float piece_size, uint piece_width, uint piece_height, uint enable_num);
		AnimeData(const Manager & manageri, const std::string & path, float piece_size, uint piece_width, uint piece_height, uint enable_num);

		bool load(const Manager & manageri, const char * path, float piece_size, uint piece_width, uint piece_height, uint enable_num);
		bool load(const Manager & manageri, const std::string & path, float piece_size, uint piece_width, uint piece_height, uint enable_num);

		uint width() const;
		uint height() const;
		uint enable_num() const;

		bool draw_no_alpha(const D3DXMATRIX & world_mat, const D3DMATERIAL9 & material, uint frame_No) const;
		bool draw_alpha(const D3DXMATRIX & world_mat, const D3DMATERIAL9 & material, int BlendMode, uint frame_No) const;





		~AnimeData();


	protected:

		uint width_;
		uint height_;
		uint enable_num_;



		class Grid
		{
		public:
			Grid(uint width, uint height, float piece_size);
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

		bool draw(const D3DXMATRIX & world_mat, const D3DMATERIAL9 & material, uint frame_No) const;

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