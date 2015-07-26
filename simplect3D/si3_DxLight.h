#pragma once

#include "si3_popular.h"

namespace si3
{
	class DxLight
	{
	public:
		bool init(LPDIRECT3DDEVICE9 dxdevice);



	private:
		LPDIRECT3DDEVICE9 dxdevice;
	};
}