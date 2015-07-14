#pragma once

#include "popular.h"

namespace si3
{
	class Mouse
	{
	public:
		enum
		{
			left,
			middle,
			right
		};

		int x() const;
		int y() const;

		int vx() const;
		int vy() const;

		int ax() const;
		int ay() const;

		int wheel_forward() const;		// マウスホイールの今回のフレームの回転量(前方方向プラス)
		int wheel_backward() const;	// マウスホイールの今回のフレームの回転量(後方方向プラス)

		bool freeing(int id) const;
		bool pushed(int id) const;
		bool pushing(int id) const;
		bool clicked(int id) const;







		Mouse();
		bool init(LPDIRECTINPUT8 & diinterface, HWND windle);
		void update();
		~Mouse();

	private:
		bool button_statuses[2/*old, now*/][3/*button_left, button_middle, button_right*/];
		bool active_index;
		int x_;
		int y_;
		int vx_;
		int vy_;
		int ax_;
		int ay_;
		int wheel_forward_;

		LPDIRECTINPUTDEVICE8 device;
		HWND windle;
	};
}