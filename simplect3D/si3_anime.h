#pragma once

#include "si3_popular.h"
#include "si3_DisplayObject.h"
//#include <si3_ImageData.h>
#include "si3_AnimeData.h"
#include "si3_AnimeCounter.h"

namespace si3
{
	class Manager;
	class AnimeData;

	/***
	板ポリに二次元アニメーションを表示するクラス。
	*/
	class Anime :public DisplayObject
	{
	public:
		Anime();
	//	Anime(const Manager & manageri, const TCHAR * path);
	//	Anime(const Manager & manageri, const std::string & path);
		Anime(const Manager & manageri, const ::si3::AnimeData & imaged);

		// 一つの三角ポリゴンの最大の大きさをここで設定する
		// 三角ポリゴンの大きさがこれを超えないように、板ポリゴンはいくつかに分割されて管理される
		void piece_size(float value);
		float piece_size() const;

	//	bool load(const Manager & manageri, const TCHAR * path);
	//	bool load(const Manager & manageri, const std::string & path);
		void anime_data(const Manager & manageri, const ::si3::AnimeData & imaged);

		void x(float value);
		float x() const;

		void y(float value);
		float y() const;

		void z(float value);
		float z() const;

		void rot_x(float value);
		float rot_x() const;

		void rot_y(float value);
		float rot_y() const;

		void rot_z(float value);
		float rot_z() const;

		void scale(float value);
		float scale() const;

		/***
		画像サイズを得る。
		*/
		uint base_width() const;
		uint base_height() const;

		/***
		拡大率適用後の画像サイズを得る。
		*/
		float width() const;
		float height() const;

		D3DMATERIAL9 & material();
		const D3DMATERIAL9 & material() const;

		void setblend_normal();
		void setblend_alpha();
		void setblend_add();
		void setblend_sub();

		void animation();
		void animation_once();
		void resistor(int v);
		void replay();
		bool is_final_frame();
		void set(int nNo);

		/***
		アニメーションの最後のコマの番号を得る。
		*/
		int max_No();





		void draw_no_alpha() const;
		void draw_alpha() const;


		~Anime();

	protected:
		mutable D3DXMATRIX world_mat;
		const Manager * manager;

	private:
		const ::si3::AnimeData * animed;
		bool ownership;
		D3DMATERIAL9 material_;

		float x_;
		float y_;
		float z_;
		float rot_x_;
		float rot_y_;
		float rot_z_;
		float scale_;
		float piece_size_;
		int blend_mode;
		AniCounter cnt;



		void construct();
		virtual void compute_world_mat() const;
		void release();
	};
}