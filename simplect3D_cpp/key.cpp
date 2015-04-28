#include <key.h>

namespace si3
{
	bool key::init(LPDIRECTINPUT8 & diinterface, HWND windle)
	{
		ZeroMemory(keystate[0], 256);
		ZeroMemory(keystate[1], 256);

		return key_systemi.init(diinterface, windle);

	}

	void key::update()
	{
		key_systemi.update(keystate[!active]);
		active = !active;
	}

	bool key::pushing(unsigned char key) const
	{
		return (keystate[active][key] & 0x80) != 0;
	}
	bool key::freeing(unsigned char key) const
	{
		return !pushing(key);
	}

	bool key::pushed(unsigned char key) const
	{
		if ((keystate[active][key] & 0x80) == 0) return false;
		if ((keystate[!active][key] & 0x80) != 0) return false;

		return true;
	}
	bool key::clicked(unsigned char key) const
	{
		if ((keystate[active][key] & 0x80) != 0) return false;
		if ((keystate[!active][key] & 0x80) == 0) return false;

		return true;
	}


}