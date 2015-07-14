#pragma once

#include "popular.h"
#include "utility.h"
#include "model_coor.h"
#include "matrix.h"
#include "model_data.h"
#include "bunish_data.h"
#include "display_object.h"
#include <list>

namespace si3
{
	class manager;
	class model;

	class bunisher :public display_object
	{
	public:
		bunisher(const manager & manageri, const model & model_);

		void init(const manager & manageri, const model & model_);

		void bunish();


		bunisher();
		~bunisher();

		void draw_no_alpha() const;
		void draw_alpha() const;

	private:
		
		struct attbute
		{
			D3DMATERIAL9 material;
			LPDIRECT3DTEXTURE9 texture;
			uint use_top_num;
		};


#if 0
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
#endif

		const manager * si3m;
		LPDIRECT3DDEVICE9 device;
		IDirect3DVertexBuffer9 * listbuff;
	//	fw::Array<attbute> attbute_list;
	//	IDirect3DIndexBuffer9 * indexbuff;

		unsigned long triangle_num;
		std::list<bunish_data> bunish_data_list;
		float alpha;
	//	bool with_alpha;

		void construct();
		void release();

		bool create_buffer(unsigned long buffer_size);
		void convert_without_index(top_type * planebuff, unsigned short * indexbuff, unsigned long index_num);
	//	void set_bunish_vec_list(top_type * top_head, unsigned long top_No);

	};
}