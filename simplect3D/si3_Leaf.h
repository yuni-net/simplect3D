#pragma once

#include "si3_popular.h"
#include "si3_DisplayObject.h"
#include "si3_Matrix.h"
//#include <si3_ImageData.h>

namespace si3
{
	class Manager;
	class ImageData;

	class Leaf :public DisplayObject
	{
	public:
		Leaf();
		Leaf(const TCHAR * path);
		Leaf(const std::string & path);
		Leaf(const ::si3::ImageData & imaged);

		// 一つの三角ポリゴンの最大の大きさをここで設定する
		// 三角ポリゴンの大きさがこれを超えないように、板ポリゴンはいくつかに分割されて管理される
		void piece_size(float value);
		float piece_size() const;

		bool load(const TCHAR * path);
		bool load(const std::string & path);
		void image_data(const ::si3::ImageData & imaged);

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

		void culling(bool is_enable);

		void affine(const Matrix & world_mat);
		const Matrix & affine() const;




		void draw_no_alpha() const;
		void draw_alpha() const;


		~Leaf();

	protected:
		mutable Matrix world_mat;

	private:
		const ::si3::ImageData * imaged;
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
		bool is_culling_on;
		mutable bool computed_world_mat;


		void construct();
		virtual void compute_world_mat() const;
		void release();
	};
}