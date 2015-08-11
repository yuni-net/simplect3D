#ifndef si3_Seal_h_
#define si3_Seal_h_

#include "si3_DisplayObject.h"
#include "si3_popular.h"
#include "si3_Matrix.h"

namespace si3
{
	class Manager;
	class SealData;

	class Seal :public DisplayObject
	{
	public:
		float x;
		float y;
		float z;
		float radian;
		float scale_x;
		float scale_y;

		Seal();
		Seal(const TCHAR * path);
		Seal(const std::string & path);
		Seal(const ::si3::SealData & imaged);

		// ˆê‚Â‚ÌOŠpƒ|ƒŠƒSƒ“‚ÌÅ‘å‚Ì‘å‚«‚³‚ğ‚±‚±‚Åİ’è‚·‚é
		// OŠpƒ|ƒŠƒSƒ“‚Ì‘å‚«‚³‚ª‚±‚ê‚ğ’´‚¦‚È‚¢‚æ‚¤‚ÉA”Âƒ|ƒŠƒSƒ“‚Í‚¢‚­‚Â‚©‚É•ªŠ„‚³‚ê‚ÄŠÇ—‚³‚ê‚é
		void piece_size(float value);
		float piece_size() const;

		bool load(const TCHAR * path);
		bool load(const std::string & path);
		void set_seal_data(const ::si3::SealData & imaged);


		void set_scale(float value);

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


		void affine(const Matrix & world_mat);
		const Matrix & affine() const;




		void draw_no_alpha() const;
		void draw_alpha() const;


		~Seal();

	protected:
		mutable Matrix world_mat;

	private:
		const ::si3::SealData * imaged;
		bool ownership;
		D3DMATERIAL9 material_;

		float piece_size_;
		int blend_mode;
		mutable bool computed_world_mat;


		void construct();
		virtual void compute_world_mat() const;
		void release();
	};
}

#endif