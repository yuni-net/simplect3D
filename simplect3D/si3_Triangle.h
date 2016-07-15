#pragma once

#include <si3_coor3.h>
#include <si3_DisplayObject.h>

namespace si3
{
	class Triangle :public DisplayObject
	{
	public:
		float x;
		float y;
		float z;

		Triangle(const Coor3 & a, const Coor3 & b, const Coor3 & c);

		const Coor3 & get_top(const int index) const;












		void draw_no_alpha() const;
		void draw_alpha() const;

	private:
		IDirect3DVertexBuffer9 * vertbuff;
		LPDIRECT3DTEXTURE9 texture;
		Coor3 tops[3];
	};
}