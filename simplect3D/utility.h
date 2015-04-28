#pragma once

#include <string>
#include "popular.h"
#include "si3_blendmode.h"
#include "si3_coor3.h"
#include "si3_top_type.h"

namespace si3
{
	const float pi = 3.14159265f;

#ifdef _UNICODE
	typedef std::wstring tstring;
#else
	typedef std::string tstring;
#endif





	void vector_to_unit_vector(const D3DVECTOR & from, D3DVECTOR & to);

	template<class T>
	void saferelease(T * pointer)
	{
		if (pointer != nullptr)
		{
			pointer->Release();
			pointer = nullptr;
		}
	}

	uchar round_uchar(float fVal);


	coor3 normalize_vector(const coor3 & base);
	coor3 triangle_gravity_center(const D3DVECTOR & pos0, const D3DVECTOR & pos1, const D3DVECTOR & pos2);
}