#pragma once

#include "popular.h"

namespace si3
{
	/***
	@brief 回転行列の逆行列を求めます。
	*/
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
			float _20, float _21, float _22 );
		Matrix(const Matrix & mat);
		Matrix(const D3DXMATRIX & dxmat);
		Matrix & operator=(const Matrix & mat);
		Matrix & operator=(const D3DXMATRIX & dxmat);
		float operator()(unsigned int y, unsigned int x);
		Matrix operator*(const Matrix & mat) const;
		Matrix & operator*=(const Matrix & mat);

		void x(float value);
		void y(float value);
		void z(float value);
		float  x() const;
		float  y() const;
		float  z() const;
		float  w() const;

		// 自身の行列を設定する。自身の行列にさらに掛け合わせるわけではない **************

		Matrix & parallel(float vx, float vy, float vz);
		Matrix & scale(float magni_x, float magni_y, float magni_z);
		Matrix & rotate(float radian);			// rotate_zと等価
		Matrix & rotate(float sin, float cos);	// rotate_zと等価
		Matrix & affine(const Matrix & scale, const Matrix & rotate, const Matrix & trans);

		Matrix & rotate_x(float radian);
		Matrix & rotate_y(float radian);
		Matrix & rotate_z(float radian);

		Matrix & rotate_x(float sin, float cos);
		Matrix & rotate_y(float sin, float cos);
		Matrix & rotate_z(float sin, float cos);

		// *******************************************************************************

		const D3DXMATRIX & dxmat() const;
		const D3DXMATRIX * dxpointer() const;

	private:
		D3DXMATRIX matrix_;


	};
}

