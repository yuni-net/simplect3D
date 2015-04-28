#pragma once

#include "popular.h"

namespace si3
{
	D3DXMATRIX inverse_matrix_of_rot(const D3DXMATRIX & rot);


	/*
	 * �|���Z�͒ʏ퉉�Z�̂Ƃ��Ɠ������ԂŋL�q���ėǂ��B
	 * �f�t�H���g�ł͒P�ʍs��ŏ����������B
	 */
	class matrix
	{
	public:
		matrix();
		matrix(
			float _00, float _01, float _02,
			float _10, float _11, float _12,
			float _20, float _21, float _22 );
		matrix(const matrix & mat);
		matrix(const D3DXMATRIX & dxmat);
		matrix & operator=(const matrix & mat);
		matrix & operator=(const D3DXMATRIX & dxmat);
		float operator()(unsigned int y, unsigned int x);
		matrix operator*(const matrix & mat) const;
		matrix & operator*=(const matrix & mat);

		void x(float fValue);
		void y(float fValue);
		void z(float fValue);
		float  x() const;
		float  y() const;
		float  z() const;
		float  w() const;

		// ���g�̍s���ݒ肷��B���g�̍s��ɂ���Ɋ|�����킹��킯�ł͂Ȃ� **************

		matrix & parallel(float fVX, float fVY, float fVZ);
		matrix & scale(float fMagniX, float fMagniY, float fMagniZ);
		matrix & rotate(float fRadian);			// rotate_z�Ɠ���
		matrix & rotate(float fSin, float fCos);	// rotate_z�Ɠ���
		matrix & affine(const matrix & scale, const matrix & rotate, const matrix & trans);

		matrix & rotate_x(float fRadian);
		matrix & rotate_y(float fRadian);
		matrix & rotate_z(float fRadian);

		matrix & rotate_x(float fSin, float fCos);
		matrix & rotate_y(float fSin, float fCos);
		matrix & rotate_z(float fSin, float fCos);

		// *******************************************************************************

		const D3DXMATRIX & dxmat() const;
		const D3DXMATRIX * dxpointer() const;

	private:
		D3DXMATRIX matrix_;


	};
}

