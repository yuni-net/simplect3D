#include <si3_Key.h>

namespace si3
{
	bool Key::init(LPDIRECTINPUT8 & diinterface, HWND windle)
	{
		ZeroMemory(keystate[0], 256);
		ZeroMemory(keystate[1], 256);

		return key_systemi.init(diinterface, windle);

	}

	void Key::update()
	{
		key_systemi.update(keystate[!active]);
		active = !active;
	}

	bool Key::pushing(unsigned char Key) const
	{
		return (keystate[active][Key] & 0x80) != 0;
	}
	bool Key::freeing(unsigned char Key) const
	{
		return !pushing(Key);
	}

	bool Key::pushed(unsigned char Key) const
	{
		if ((keystate[active][Key] & 0x80) == 0) return false;
		if ((keystate[!active][Key] & 0x80) != 0) return false;

		return true;
	}
	bool Key::clicked(unsigned char Key) const
	{
		if ((keystate[active][Key] & 0x80) != 0) return false;
		if ((keystate[!active][Key] & 0x80) == 0) return false;

		return true;
	}


}