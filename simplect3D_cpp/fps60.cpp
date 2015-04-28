#include <fps60.h>

namespace si3
{
	void fps60::init()
	{
		timeBeginPeriod(1);
		old_ms = timeGetTime();
	}

	void fps60::stop_if_need()
	{
		static const float a_second_ms = 1000;
		static const float fps = 60;
		static const float limit_ms = a_second_ms / fps;

		while (true)
		{
			DWORD now_ms = timeGetTime();
			bool its_over_time = (now_ms - old_ms) > limit_ms;
			if (its_over_time)
			{
				old_ms = now_ms;
				break;
			}
		}
	}

	fps60::~fps60()
	{
		timeEndPeriod(1);
	}

}