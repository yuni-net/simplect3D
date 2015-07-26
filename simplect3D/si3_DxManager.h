#pragma once

#include <si3_popular.h>

namespace si3
{
	class DxManager
	{
	public:
		bool init(HWND win_hand, unsigned int screen_width, unsigned int screen_height);
		D3DPRESENT_PARAMETERS * get_present_parameters();
		LPDIRECT3DDEVICE9 get_dxdevice();
		const D3DPRESENT_PARAMETERS * get_present_parameters() const;
		const LPDIRECT3DDEVICE9 get_dxdevice() const;


		DxManager();
		~DxManager();
	private:
		LPDIRECT3D9 d3d;
		D3DPRESENT_PARAMETERS present_parameters;
		LPDIRECT3DDEVICE9 dxdevice;
	};
}