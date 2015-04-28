#pragma once

#include "popular.h"

namespace si3
{
	D3DXMATRIX inverse_matrix_of_rot(const D3DXMATRIX & rot);


	/*
	 * 掛け算は通常演算のときと同じ順番で記述して良い。
	 * デフォルトでは単位行列で初期化される。
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

		// 自身の行列を設定する。自身の行列にさらに掛け合わせるわけではない **************

		matrix & parallel(float fVX, float fVY, float fVZ);
		matrix & scale(float fMagniX, float fMagniY, float fMagniZ);
		matrix & rotate(float fRadian);			// rotate_zと等価
		matrix & rotate(float fSin, float fCos);	// rotate_zと等価
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

