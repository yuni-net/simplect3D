#include <dxinput.h>
#include <utility.h>

namespace si3
{
	bool dxinput::init(HWND windle)
	{
		HRESULT result;

		result = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&diinterface, NULL);
		if (FAILED(result))
		{
			fw::popup("failed to get iDirectInput8 interface");
			return false;
		}

		if (keyi.init(diinterface, windle) == false) return false;
		if (mousei.init(diinterface, windle) == false) return false;

		return true;
	}

	void dxinput::update()
	{
		keyi.update();
		mousei.update();
	}

	const ::si3::key & dxinput::key() const
	{
		return keyi;
	}

	const ::si3::mouse & dxinput::mouse() const
	{
		return mousei;
	}




	dxinput::dxinput()
	{
		diinterface = nullptr;
	}

	dxinput::~dxinput()
	{
		dxsaferelease(diinterface);
	}


}