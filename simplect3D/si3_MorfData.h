#pragma once

#include "si3_popular.h"
#include "si3_utility.h"
#include "si3_Matrix.h"

namespace si3
{
	class MorfData
	{
	public:
		MorfData(const tstring & before, const tstring & after);

		/**
		beforeの割合を0.0-1.0の範囲で設定して更新する。
		当然afterの割合は1.0-percentとなる。
		*/
		void update(const float percent);

		void draw_no_alpha(const Matrix & world_mat, bool is_culling) const;
		void draw_alpha(const Matrix & world_mat) const;



		~MorfData();
	private:
		struct Attbute
		{
			D3DMATERIAL9 material;
			LPDIRECT3DTEXTURE9 texture;
			uint use_top_num;
		};

		class MyModelData
		{
		public:
			fw::Array<Attbute> attbute_list;
			IDirect3DVertexBuffer9 * vertbuff;
			unsigned long buff_bytes;

			void load(const tstring & path);



		private:
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
				char texture_name[20];	// 最後の'\0'は保障されていないので注意
			};
#pragma pack(pop)

			IDirect3DVertexBuffer9 * base_tops;
			IDirect3DIndexBuffer9 * base_indexes;

			bool create_top_buffer(unsigned long top_num);
			bool create_index_buffer(unsigned long index_num);
			bool create_answer_buff(unsigned long buff_bytes);
			void load_material(D3DMATERIAL9 & mate, const Pmd_mate_data & mate_data);
			bool load_texture(LPDIRECT3DTEXTURE9 & texture, char tex_name[20], const TCHAR * path);
			bool load_header(FILE * fp);
			bool load_top(FILE * fp);
			bool load_index(FILE * fp);
			bool decompress();
			bool load_attbute(FILE * fp, const char * path);
		};

		MyModelData before;
		MyModelData after;

		IDirect3DVertexBuffer9 * vertbuff;
		fw::Array<Attbute> * now_attbute;
		unsigned long buff_bytes;
		float blight_percent;

		LPDIRECT3DDEVICE9 device;


		const DxTop & get_top(const DxTop * buffer, MyModelData & modeld, const unsigned int index);
		void multiple(const DxTop & one, const DxTop & ano, const float percent, DxTop & answer);
		void release(MyModelData & modeld);
	};
}