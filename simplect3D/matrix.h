#pragma once

#include "popular.h"

namespace si3
{
	D3DXMATRIX inverse_matrix_of_rot(const D3DXMATRIX & rot);


	/*
	* 掛け算は通常演算のときと同じ順番で記述して良い。
	* デフォルトでは単位行列で初期化される。
	*/
	class Matrix
	{
	public:
		Matrix();
		Matrix(
			float _00, float _01, float _02,
			float _10, float _11, float _12,
			float _20, float _21, float _22);
		Matrix(const Matrix & mat);
		Matrix(const D3DXMATRIX & dxmat);
		Matrix & operator=(const Matrix & mat);
		Matrix & operator=(const D3DXMATRIX & dxmat);
		float operator()(unsigned int y, unsigned int x);
		Matrix operator*(const Matrix & mat) const;
		Matrix & operator*=(const Matrix & mat);

		void x(float fValue);
		void y(float fValue);
		void z(float fValue);
		float  x() const;
		float  y() const;
		float  z() const;
		float  w() const;

		// 自身の行列を設定する。自身の行列にさらに掛け合わせるわけではない **************

		Matrix & parallel(float fVX, float fVY, float fVZ);
		Matrix & scale(float fMagniX, float fMagniY, float fMagniZ);
		Matrix & rotate(float fRadian);			// rotate_zと等価
		Matrix & rotate(float fSin, float fCos);	// rotate_zと等価
		Matrix & affine(const Matrix & scale, const Matrix & rotate, const Matrix & trans);

		Matrix & rotate_x(float fRadian);
		Matrix & rotate_y(float fRadian);
		Matrix & rotate_z(float fRadian);

		Matrix & rotate_x(float fSin, float fCos);
		Matrix & rotate_y(float fSin, float fCos);
		Matrix & rotate_z(float fSin, float fCos);

		// *******************************************************************************

		const D3DXMATRIX & dxmat() const;
		const D3DXMATRIX * dxpointer() const;

	private:
		D3DXMATRIX matrix_;


	};
}