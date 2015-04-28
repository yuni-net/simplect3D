#pragma once

#include "popular.h"
#include "key_system.h"

namespace si3
{
	class key
	{
	public:
		bool init(LPDIRECTINPUT8 & diinterface, HWND windle);
		void update();

		bool pushing(unsigned char key) const;
		bool freeing(unsigned char key) const;

		bool pushed(unsigned char key) const;
		bool clicked(unsigned char key) const;

#if 0
		int pushing_num() const;
		int freeing_num() const;
		int pushed_num() const;
		int clicked_num() const;
#endif




	private:
		BYTE keystate[2][256];
		bool active;
		key_system key_systemi;

#if 0
		int pushing_num_;
		int freeing_num_;
		int pushed_num_;
		int clicked_num_;
#endif

	};
}