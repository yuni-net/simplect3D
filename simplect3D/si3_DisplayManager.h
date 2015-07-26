#pragma once

#include "si3_popular.h"
#include "si3_DisplayObject.h"
#include "si3_SkydomeMini.h"

namespace si3
{
	typedef fw::Array<const DisplayObject *> vdisplay_object;

	class DisplayManager
	{
	public:
		void init(D3DPRESENT_PARAMETERS * present_parameters, LPDIRECT3DDEVICE9 dxdevice);
		void register_display_object(const DisplayObject & display_objecti);

		void context_color(float r, float g, float b);
		float context_red() const;
		float context_green() const;
		float context_blue() const;

		Skydome_mini & get_skydome();

		void draw();



		DisplayManager();
	private:
		vdisplay_object display_objects;
		D3DPRESENT_PARAMETERS * present_parameters;
		LPDIRECT3DDEVICE9 dxdevice;
		float red_;
		float green_;
		float blue_;
		Skydome_mini skydome;
	};
}