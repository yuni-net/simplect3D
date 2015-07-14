#pragma once

#include "popular.h"
#include "key.h"
#include "mouse.h"

namespace si3
{
	class DxInput
	{
	public:
		bool init(HWND windle);
		void update();
		const ::si3::Key & key() const;
		const ::si3::Mouse & mouse() const;




		DxInput();
		~DxInput();
	private:
		LPDIRECTINPUT8 diinterface;
		::si3::Key keyi;
		::si3::Mouse mousei;
	};
}