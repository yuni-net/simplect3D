#pragma once

#include "popular.h"
#include "utility.h"
#include "model_coor.h"
#include "Matrix.h"
#include "model_data.h"
#include "bunish_data.h"
#include "display_object.h"
#include <list>

namespace si3
{
	class Manager;
	class Model;

	/***
	3Dモデルをバラバラに分解する表現を行うクラス。
	*/
	class Bunisher :public DisplayObject
	{
	public:
		Bunisher(const Manager & manageri, const Model & model_);

		void init(const Manager & manageri, const Model & model_);

		void bunish();


		Bunisher();
		~Bunisher();

		void draw_no_alpha() const;
		void draw_alpha() const;

	private:
		
		struct Attbute
		{
			D3DMATERIAL9 material;
			LPDIRECT3DTEXTURE9 texture;
			uint use_top_num;
		};



		const Manager * si3m;
		LPDIRECT3DDEVICE9 device;
		IDirect3DVertexBuffer9 * listbuff;

		unsigned long triangle_num;
		std::list<BunishData> bunish_data_list;
		float alpha;

		void construct();
		void release();

		bool create_buffer(unsigned long buffer_size);
		void convert_without_index(DxTop * planebuff, unsigned short * indexbuff, unsigned long index_num);

	};
}