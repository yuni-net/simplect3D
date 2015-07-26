#include <si3_KeySystem.h>

namespace si3
{
	bool Key_system::init(LPDIRECTINPUT8 & diinterface, HWND windle)
	{
		this->windle = windle;
		this->diinterface = diinterface;

		HRESULT result;

		result = diinterface->CreateDevice(GUID_SysKeyboard, &device, NULL);
		if (FAILED(result))
		{
			fw::popup("failed to get IDirectInputDevice8 interface");
			return false;
		}

		result = device->SetDataFormat(&c_dfDIKeyboard);
		if (FAILED(result))
		{
			fw::popup("failed to set data format");
			return false;
		}

		result = device->SetCooperativeLevel(windle, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
		if (FAILED(result))
		{
			fw::popup("failed to set cooperative level");
			return false;
		}

		device->Acquire();

		return true;
	}

	void Key_system::update(BYTE * keys)
	{
		HRESULT result;

		// getting direct data
		result = device->GetDeviceState(256, keys);
		if (FAILED(result))
		{
			device->Acquire();
			result = device->GetDeviceState(256, keys);
			if (result == DIERR_INPUTLOST)
			{
				release();
				init(diinterface, windle);
				result = device->GetDeviceState(256, keys);
				if (FAILED(result))
				{
					fw::popup("fatal error in dx9key");
				}
			}
		}

	}



	Key_system::Key_system()
	{
		device = nullptr;
	}

	Key_system::~Key_system()
	{
		release();
	}

	void Key_system::release()
	{
		if (device)
		{
			device->Unacquire();
			device->Release();
			device = NULL;
		}

	}
}