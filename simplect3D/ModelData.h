#pragma once

#include "popular.h"
#include "utility.h"
#include "model_coor.h"
#include "matrix.h"
#include "model_data.h"
#include "utility.h"
#include "BoneBranch.h"

namespace si3
{
	class manager;
	class MotionData;
	class BoneMap;

	class ModelData
	{
	public:
		ModelData(const manager & manageri, const TCHAR * path);
		ModelData(const manager & manageri, const tstring & path);
		ModelData(const manager & manageri, const TCHAR * path, const si3::coor3 & center);

		bool load(const manager & manageri, const TCHAR * path, MotionData & motion_data, BoneMap & bone_map);
		bool load(const manager & manageri, const TCHAR * path);
		bool load(const manager & manageri, const tstring & path);
		bool load(const manager & manageri, const TCHAR * path, const si3::coor3 & center);

		unsigned long index_num() const;
		top_type * lock_top_buffer() const;
		unsigned short * lock_index_buffer() const;
		void unlock_top_buffer() const;
		void unlock_index_buffer() const;


		int bone_num() const;


		void animation(MotionData & motion_data);

		ModelData();
		~ModelData();

		void draw_no_alpha() const;
		void draw_alpha() const;

	private:

		struct attbute
		{
			D3DMATERIAL9 material;
			LPDIRECT3DTEXTURE9 texture;
			uint use_top_num;
		};

#pragma pack(push, 1)
		struct top_data
		{
			float pos[3];
			float normal[3];
			float uv[2];
			unsigned short bone_num[2];
			uchar bone_weight;
			uchar edge_flag;
		};

#pragma pack(pop)

#pragma pack(push, 1)

		struct pmd_mate_data
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
		fw::Array<attbute> attbute_list;
		IDirect3DVertexBuffer9 * vertbuff;
		IDirect3DIndexBuffer9 * indexbuff;
		unsigned long index_num_;
		fw::Array<BoneBranch> bone_tree;
		int bone_num_;

		void construct();
		void release();

		bool load_header(FILE * fp);
		bool load_bone(FILE * fp, BoneMap & bone_map);
		bool create_top_buffer(unsigned long top_num);
		bool load_top_center(FILE * fp, const si3::coor3 & center);
		bool load_top(FILE * fp);
		bool load_top(FILE * fp, MotionData & motion_data);
		bool create_index_buffer(unsigned long index_num);
		bool load_index(FILE * fp);
		void load_material(D3DMATERIAL9 & mate, const pmd_mate_data & mate_data);
		bool load_texture(LPDIRECT3DTEXTURE9 & texture, char tex_name[20], const TCHAR * path);
		bool load_attbute(FILE * fp, const TCHAR * path);

	};
}