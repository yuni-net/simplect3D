#pragma once

#include "popular.h"
#include "display_object.h"

namespace si3
{
	typedef fw::vector<const display_object *> vdisplay_object;

	class display_manager
	{
	public:
		void init(D3DPRESENT_PARAMETERS * present_parameters, LPDIRECT3DDEVICE9 dxdevice);
		void register_display_object(const display_object & display_objecti);

		void context_color(float r, float g, float b);
		float context_red() const;
		float context_green() const;
		float context_blue() const;

		void draw();



		display_manager();
	private:
		vdisplay_object display_objects;
		D3DPRESENT_PARAMETERS * present_parameters;
		LPDIRECT3DDEVICE9 dxdevice;
		float fRed_;
		float fGreen_;
		float fBlue_;
	};
}