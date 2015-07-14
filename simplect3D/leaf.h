#pragma once

#include "popular.h"
#include "display_object.h"
#include "matrix.h"
//#include <image_data.h>

namespace si3
{
	class Manager;
	class image_data;

	class leaf :public display_object
	{
	public:
		leaf();
		leaf(const Manager & manageri, const TCHAR * path);
		leaf(const Manager & manageri, const std::string & path);
		leaf(const Manager & manageri, const ::si3::image_data & imaged);

		// ˆê‚Â‚ÌOŠpƒ|ƒŠƒSƒ“‚ÌÅ‘å‚Ì‘å‚«‚³‚ğ‚±‚±‚Åİ’è‚·‚é
		// OŠpƒ|ƒŠƒSƒ“‚Ì‘å‚«‚³‚ª‚±‚ê‚ğ’´‚¦‚È‚¢‚æ‚¤‚ÉA”Âƒ|ƒŠƒSƒ“‚Í‚¢‚­‚Â‚©‚É•ªŠ„‚³‚ê‚ÄŠÇ—‚³‚ê‚é
		void piece_size(float value);
		float piece_size() const;

		bool load(const Manager & manageri, const TCHAR * path);
		bool load(const Manager & manageri, const std::string & path);
		void image_data(const Manager & manageri, const ::si3::image_data & imaged);

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

		void culling(bool bEnable);

		void affine(const matrix & world_mat);
		const matrix & affine() const;




		void draw_no_alpha() const;
		void draw_alpha() const;


		~leaf();

	protected:
		mutable matrix world_mat;
		const Manager * pManager;

	private:
		const ::si3::image_data * imaged;
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
		int BlendMode;
		bool bCulling;
		mutable bool bComputedWorldMat;


		void construct();
		virtual void compute_world_mat() const;
		void release();
	};
}