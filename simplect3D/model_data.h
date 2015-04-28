#pragma once

#include "popular.h"
#include "utility.h"
#include "model_coor.h"
#include "matrix.h"
#include "model_data.h"
#include "utility.h"

namespace si3
{
	class manager;

	class model_data
	{
	public:
		model_data(const manager & manageri, const TCHAR * path);
		model_data(const manager & manageri, const tstring & path);
		model_data(const manager & manageri, const TCHAR * path, const si3::coor3 & center);

		bool load(const manager & manageri, const TCHAR * path);
		bool load(const manager & manageri, const tstring & path);
		bool load(const manager & manageri, const TCHAR * path, const si3::coor3 & center);

		unsigned long index_num() const;
		top_type * lock_top_buffer() const;
		unsigned short * lock_index_buffer() const;
		void unlock_top_buffer() const;
		void unlock_index_buffer() const;


		model_data();
		~model_data();

		void draw_no_alpha(const matrix & world_mat, bool is_culling) const;
		void draw_alpha(const matrix & world_mat) const;

	private:

		struct attbute
		{
			D3DMATERIAL9 material;
			LPDIRECT3DTEXTURE9 texture;
			uint use_top_num;
		};

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
		fw::vector<attbute> attbute_list;
		IDirect3DVertexBuffer9 * vertbuff;
		IDirect3DIndexBuffer9 * indexbuff;
		unsigned long index_num_;
		//	bool with_alpha;

		void construct();
		void release();

		bool load_header(FILE * fp);
		bool create_top_buffer(unsigned long top_num);
		bool load_top_center(FILE * fp, const si3::coor3 & center);
		bool load_top(FILE * fp);
		bool create_index_buffer(unsigned long index_num);
		bool load_index(FILE * fp);
		void load_material(D3DMATERIAL9 & mate, const pmd_mate_data & mate_data);
		bool load_texture(LPDIRECT3DTEXTURE9 & texture, char tex_name[20], const TCHAR * path);
		bool load_attbute(FILE * fp, const TCHAR * path);

#if 0
		//	void compute_world_mat(D3DXMATRIX & mat, const model_coor & coor) const;
		void add_normal(LPD3DXMESH & mesh, const LPDIRECT3DDEVICE9 & device, LPD3DXMESH & pMeshWk);
		bool get_table(LPD3DXMESH & pMeshWk, LPD3DXBUFFER & pMtrlBuf);
		void compute_tops(LPD3DXMESH & mesh);
		void compute_tops_center(LPD3DXMESH & mesh, const coor3 & center);
		void get_material(const TCHAR * path, LPD3DXBUFFER	pMtrlBuf, const LPDIRECT3DDEVICE9 & device);
		void judge_with_alpha();
#endif
	};
}