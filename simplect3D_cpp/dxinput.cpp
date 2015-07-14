#include <dxinput.h>
#include <utility.h>

namespace si3
{
	bool DxInput::init(HWND windle)
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

	void DxInput::update()
	{
		keyi.update();
		mousei.update();
	}

	const ::si3::Key & DxInput::key() const
	{
		return keyi;
	}

	const ::si3::Mouse & DxInput::mouse() const
	{
		return mousei;
	}




	DxInput::DxInput()
	{
		diinterface = nullptr;
	}

	DxInput::~DxInput()
	{
		dxsaferelease(diinterface);
	}


}