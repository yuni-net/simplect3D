#pragma once

#include "popular.h"

namespace si3
{
	class Key_system
	{
	public:
		bool init(LPDIRECTINPUT8 & diinterface, HWND windle);
		void update(BYTE * keys);



		Key_system();
		~Key_system();
	private:
		LPDIRECTINPUT8 diinterface;
		LPDIRECTINPUTDEVICE8 device;
		HWND windle;

		void release();
	};
}