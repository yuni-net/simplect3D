#pragma once

#include "si3_popular.h"
#include "si3_KeySystem.h"

namespace si3
{
	class Key
	{
	public:
		bool init(LPDIRECTINPUT8 & diinterface, HWND windle);
		void update();

		bool pushing(unsigned char key) const;
		bool freeing(unsigned char key) const;

		bool pushed(unsigned char key) const;
		bool clicked(unsigned char key) const;





	private:
		BYTE keystate[2][256];
		bool active;
		Key_system key_systemi;


	};
}