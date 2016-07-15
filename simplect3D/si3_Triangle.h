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
		void draw_no_alpha() const;
		void draw_alpha() const;








	private:
		IDirect3DVertexBuffer9 * vertbuff;
	};
}