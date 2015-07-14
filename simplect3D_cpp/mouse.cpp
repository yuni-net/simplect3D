#include <mouse.h>
#include <utility.h>

namespace si3
{
	int Mouse::x() const
	{
		return x_;
	}
	int Mouse::y() const
	{
		return y_;
	}

	int Mouse::vx() const
	{
		return vx_;
	}
	int Mouse::vy() const
	{
		return vy_;
	}

	int Mouse::ax() const
	{
		return ax_;
	}
	int Mouse::ay() const
	{
		return ay_;
	}

	int Mouse::wheel_forward() const	// マウスホイールの今回のフレームの回転量(前方方向プラス)
	{
		return wheel_forward_;
	}
	int Mouse::wheel_backward() const	// マウスホイールの今回のフレームの回転量(後方方向プラス)
	{
		return 0 - wheel_forward_;	// 符号反転
	}

	bool Mouse::freeing(int id) const
	{
		return button_statuses[active_index][id] == false;
	}
	bool Mouse::pushed(int id) const
	{
		return
			button_statuses[!active_index][id] == false
			&&
			button_statuses[active_index][id] == true;
	}
	bool Mouse::pushing(int id) const
	{
		return button_statuses[active_index][id] == true;
	}
	bool Mouse::clicked(int id) const
	{
		return
			button_statuses[!active_index][id] == true
			&&
			button_statuses[active_index][id] == false;
	}


	Mouse::Mouse()
	{
		device = nullptr;
	}
	bool Mouse::init(LPDIRECTINPUT8 & diinterface, HWND windle)
	{
		this->windle = windle;

		active_index = 0;
		for (int n2 = 0; n2 < 2; ++n2)
		{
			for (int n3 = 0; n3 < 3; ++n3)
			{
				button_statuses[n2][n3] = false;
			}
		}

		vx_ = 0;
		vy_ = 0;
		ax_ = 0;
		ay_ = 0;

		HRESULT result = diinterface->CreateDevice(GUID_SysMouse, &device, NULL);
		if (FAILED(result))
		{
			fw::popup("failed to create device of Mouse");
			return false;
		}

		result = device->SetDataFormat(&c_dfDIMouse2);
		if (FAILED(result))
		{
			fw::popup("failed to set data-format of Mouse");
			return false;
		}

		device->Acquire();

		return true;
	}
	void Mouse::update()
	{
		DIMOUSESTATE2 dims;
		HRESULT result;

		result = device->GetDeviceState(sizeof(DIMOUSESTATE2), &dims);

		if (SUCCEEDED(result))
		{
			active_index = !active_index;
			button_statuses[active_index][left] = (dims.rgbButtons[0] & (1 << 7)) != 0;
			button_statuses[active_index][right] = (dims.rgbButtons[1] & (1 << 7)) != 0;
			button_statuses[active_index][middle] = (dims.rgbButtons[2] & (1 << 7)) != 0;

			wheel_forward_ = dims.lZ;

			POINT newxy;

			GetCursorPos(&newxy);
			ScreenToClient(windle, &newxy);

			int old_vx = vx_;
			int old_vy = vy_;
			vx_ = newxy.x - x_;
			vy_ = newxy.y - y_;
			x_ = newxy.x;
			y_ = newxy.y;
			ax_ = vx_ - old_vx;
			ay_ = vy_ - old_vy;
		}
		else
		{
			if (result == DIERR_INPUTLOST)
			{
				device->Acquire();
			}
		}
	}

	Mouse::~Mouse()
	{
		dxsaferelease(device);
	}

}