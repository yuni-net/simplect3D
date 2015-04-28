#pragma once

#include "popular.h"

namespace si3
{
	class key_system
	{
	public:
		bool init(LPDIRECTINPUT8 & diinterface, HWND windle);
		void update(BYTE * keys);



		key_system();
		~key_system();
	private:
		LPDIRECTINPUT8 diinterface;
		LPDIRECTINPUTDEVICE8 device;
		HWND windle;

		void release();
	};
}