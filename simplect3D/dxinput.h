#pragma once

#include "popular.h"
#include "key.h"
#include "mouse.h"

namespace si3
{
	class dxinput
	{
	public:
		bool init(HWND windle);
		void update();
		const ::si3::key & key() const;
		const ::si3::mouse & mouse() const;




		dxinput();
		~dxinput();
	private:
		LPDIRECTINPUT8 diinterface;
		::si3::key keyi;
		::si3::mouse mousei;
	};
}