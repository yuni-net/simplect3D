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
	@brief �x�N�g���𒷂�1�̃x�N�g���ɕϊ����܂��B
	@param
	    from: �ϊ��O�̃x�N�g�����w�肵�܂��B
		to: �ϊ���̃x�N�g�����󂯎��ϐ��������Ɏw�肵�܂��B
	*/
	void vector_to_unit_vector(const D3DVECTOR & from, D3DVECTOR & to);


	/***
	@brief �������ۂ߂�unsigned char�̒l�͈͓̔��Ɏ��߂܂��B
	*/
	uchar round_uchar(float fVal);


	/***
	@brief �x�N�g���𒷂�1�̃x�N�g���ɕϊ����܂��B
	@param �ϊ��O�̃x�N�g�����w�肵�܂��B
	@return �ϊ���̃x�N�g�����Ԃ�܂��B
	*/
	Coor3 normalize_vector(const Coor3 & base);

	/***
	�O�p�`�̏d�S�����߂܂��B
	*/
	Coor3 triangle_gravity_center(const D3DVECTOR & pos0, const D3DVECTOR & pos1, const D3DVECTOR & pos2);
}