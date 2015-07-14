#pragma once

#include "popular.h"
#include "display_object.h"
//#include <image_data.h>
#include "anime_data.h"
#include "canimer.h"

namespace si3
{
	class Manager;
	class anime_data;

	class anime :public display_object
	{
	public:
		anime();
	//	anime(const Manager & manageri, const TCHAR * path);
	//	anime(const Manager & manageri, const std::string & path);
		anime(const Manager & manageri, const ::si3::anime_data & imaged);

		// 一つの三角ポリゴンの最大の大きさをここで設定する
		// 三角ポリゴンの大きさがこれを超えないように、板ポリゴンはいくつかに分割されて管理される
		void piece_size(float value);
		float piece_size() const;

	//	bool load(const Manager & manageri, const TCHAR * path);
	//	bool load(const Manager & manageri, const std::string & path);
		void anime_data(const Manager & manageri, const ::si3::anime_data & imaged);

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

		uint base_width() const;
		uint base_height() const;

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
		bool ifFinalFrame();
		void set(int nNo);
		int maxNo();





		void draw_no_alpha() const;
		void draw_alpha() const;


		~anime();

	protected:
		mutable D3DXMATRIX world_mat;
		const Manager * pManager;

	private:
		const ::si3::anime_data * animed;
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
		int nBlendmode;
		canimer cnt;



		void construct();
		virtual void compute_world_mat() const;
		void release();
	};
}