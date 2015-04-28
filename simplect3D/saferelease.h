#pragma once

namespace si3
{

	template<typename T>
	void saferelease(T * p)
	{
		if (p)
		{
			delete p;
		}

		p = nullptr;
	}

	template<typename T>
	void dxsaferelease(T * p)
	{
		if (p)
		{
			p->Release();
		}

		p = nullptr;
	}

}