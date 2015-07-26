#pragma once

#include "si3_popular.h"
#include "si3_utility.h"
#include "si3_ModelCoor.h"
#include "si3_Matrix.h"
#include "si3_utility.h"

namespace si3
{
	class Manager;

	class ModelData
	{
	public:
		ModelData(const Manager & manageri, const TCHAR * path);
		ModelData(const Manager & manageri, const tstring & path);
		ModelData(const Manager & manageri, const TCHAR * path, const si3::Coor3 & center);

		bool load(const Manager & manageri, const TCHAR * path);
		bool load(const Manager & manageri, const tstring & path);
		bool load(const Manager & manageri, const TCHAR * path, const si3::Coor3 & center);

		unsigned long index_num() const;
		DxTop * lock_top_buffer() const;
		unsigned short * lock_index_buffer() const;
		void unlock_top_buffer() const;
		void unlock_index_buffer() const;


		ModelData();
		~ModelData();

		void draw_no_alpha(const Matrix & world_mat, bool is_culling) const;
		void draw_alpha(const Matrix & world_mat) const;

	private:

		struct Attbute
		{
			D3DMATERIAL9 material;
			LPDIRECT3DTEXTURE9 texture;
			uint use_top_num;
		};

#pragma pack(push, 1)

		struct Pmd_mate_data
		{
			float diffuse[3];
			float alpha;
			float spec_power;
			float specular[3];
			float ambient[3];
			uchar toon_index;
			uchar edge_flag;
			unsigned long use_top_num;
			char texture_name[20];	// ç≈å„ÇÃ'\0'ÇÕï€è·Ç≥ÇÍÇƒÇ¢Ç»Ç¢ÇÃÇ≈íçà”
		};

#pragma pack(pop)

		LPDIRECT3DDEVICE9 device;
		fw::Array<Attbute> attbute_list;
		IDirect3DVertexBuffer9 * vertbuff;
		IDirect3DIndexBuffer9 * indexbuff;
		unsigned long index_num_;
		//	bool with_alpha;

		void construct();
		void release();

		bool load_header(FILE * fp);
		bool create_top_buffer(unsigned long top_num);
		bool load_top_center(FILE * fp, const si3::Coor3 & center);
		bool load_top(FILE * fp);
		bool create_index_buffer(unsigned long index_num);
		bool load_index(FILE * fp);
		void load_material(D3DMATERIAL9 & mate, const Pmd_mate_data & mate_data);
		bool load_texture(LPDIRECT3DTEXTURE9 & texture, char tex_name[20], const TCHAR * path);
		bool load_attbute(FILE * fp, const TCHAR * path);

	};
}