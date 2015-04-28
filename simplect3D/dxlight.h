#pragma once

#include "popular.h"

namespace si3
{
	class dxlight
	{
	public:
		bool init(LPDIRECT3DDEVICE9 dxdevice);



	private:
		LPDIRECT3DDEVICE9 dxdevice;
	};
}