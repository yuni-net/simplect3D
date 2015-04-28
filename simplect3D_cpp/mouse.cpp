#include <mouse.h>
#include <utility.h>

namespace si3
{
	int mouse::x() const
	{
		return x_;
	}
	int mouse::y() const
	{
		return y_;
	}

	int mouse::vx() const
	{
		return vx_;
	}
	int mouse::vy() const
	{
		return vy_;
	}

	int mouse::ax() const
	{
		return ax_;
	}
	int mouse::ay() const
	{
		return ay_;
	}

	int mouse::wheel_forward() const	// マウスホイールの今回のフレームの回転量(前方方向プラス)
	{
		return wheel_forward_;
	}
	int mouse::wheel_backward() const	// マウスホイールの今回のフレームの回転量(後方方向プラス)
	{
		return 0 - wheel_forward_;	// 符号反転
	}

	bool mouse::freeing(int id) const
	{
		return abButtonStatus[bActiveIndex][id] == false;
	}
	bool mouse::pushed(int id) const
	{
		return
			abButtonStatus[!bActiveIndex][id] == false
			&&
			abButtonStatus[bActiveIndex][id] == true;
	}
	bool mouse::pushing(int id) const
	{
		return abButtonStatus[bActiveIndex][id] == true;
	}
	bool mouse::clicked(int id) const
	{
		return
			abButtonStatus[!bActiveIndex][id] == true
			&&
			abButtonStatus[bActiveIndex][id] == false;
	}


	mouse::mouse()
	{
		device = nullptr;
	}
	bool mouse::init(LPDIRECTINPUT8 & diinterface, HWND windle)
	{
		this->windle = windle;

		bActiveIndex = 0;
		for (int n2 = 0; n2 < 2; ++n2)
		{
			for (int n3 = 0; n3 < 3; ++n3)
			{
				abButtonStatus[n2][n3] = false;
			}
		}

		vx_ = 0;
		vy_ = 0;
		ax_ = 0;
		ay_ = 0;

		HRESULT result = diinterface->CreateDevice(GUID_SysMouse, &device, NULL);
		if (FAILED(result))
		{
			fw::popup("failed to create device of mouse");
			return false;
		}

		result = device->SetDataFormat(&c_dfDIMouse2);
		if (FAILED(result))
		{
			fw::popup("failed to set data-format of mouse");
			return false;
		}

		device->Acquire();

		return true;
	}
	void mouse::update()
	{
		DIMOUSESTATE2 dims;
		HRESULT result;

		result = device->GetDeviceState(sizeof(DIMOUSESTATE2), &dims);

		if (SUCCEEDED(result))
		{
			bActiveIndex = !bActiveIndex;
			abButtonStatus[bActiveIndex][left] = (dims.rgbButtons[0] & (1 << 7)) != 0;
			abButtonStatus[bActiveIndex][right] = (dims.rgbButtons[1] & (1 << 7)) != 0;
			abButtonStatus[bActiveIndex][middle] = (dims.rgbButtons[2] & (1 << 7)) != 0;

			wheel_forward_ = dims.lZ;

			POINT newxy;

			GetCursorPos(&newxy);
			ScreenToClient(windle, &newxy);

			int nOldVX = vx_;
			int nOldVY = vy_;
			vx_ = newxy.x - x_;
			vy_ = newxy.y - y_;
			x_ = newxy.x;
			y_ = newxy.y;
			ax_ = vx_ - nOldVX;
			ay_ = vy_ - nOldVY;
		}
		else
		{
			if (result == DIERR_INPUTLOST)
			{
				device->Acquire();
			}
		}
	}

	mouse::~mouse()
	{
		saferelease(device);
	}

}