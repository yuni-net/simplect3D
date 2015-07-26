#pragma once

#include <string>
#include "si3_popular.h"
#include "si3_blendmode.h"
#include "si3_coor3.h"
#include "si3_DxTop.h"
#include "si3_Quaternion.h"
#include "si3_saferelease.h"

namespace si3
{
	const float pi = 3.14159265f;

#ifdef _UNICODE
	typedef std::wstring tstring;
#else
	typedef std::string tstring;
#endif



	/***
	@brief ベクトルを長さ1のベクトルに変換します。
	@param
	    from: 変換前のベクトルを指定します。
		to: 変換後のベクトルを受け取る変数をここに指定します。
	*/
	void vector_to_unit_vector(const D3DVECTOR & from, D3DVECTOR & to);


	/***
	@brief 実数を丸めてunsigned charの値の範囲内に収めます。
	*/
	uchar round_uchar(float fVal);


	/***
	@brief ベクトルを長さ1のベクトルに変換します。
	@param 変換前のベクトルを指定します。
	@return 変換後のベクトルが返ります。
	*/
	Coor3 normalize_vector(const Coor3 & base);

	/***
	三角形の重心を求めます。
	*/
	Coor3 triangle_gravity_center(const D3DVECTOR & pos0, const D3DVECTOR & pos1, const D3DVECTOR & pos2);
}