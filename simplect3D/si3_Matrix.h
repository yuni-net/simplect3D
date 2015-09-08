#pragma once

#include "si3_popular.h"

namespace si3
{
	D3DXMATRIX inverse_matrix_of_rot(const D3DXMATRIX & rot);


	struct Coor3;

	/*
	* �|���Z�͒ʏ퉉�Z�̂Ƃ��Ɠ������ԂŋL�q���ėǂ��B
	* �f�t�H���g�ł͒P�ʍs��ŏ����������B
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
		Matrix(const Coor3 & coor);
		Matrix & operator=(const Matrix & mat);
		Matrix & operator=(const D3DXMATRIX & dxmat);
		Matrix & operator=(const Coor3 & coor);
		float operator()(unsigned int y, unsigned int x);
		float operator()(unsigned int y, unsigned int x) const;
		Matrix operator*(const Matrix & mat) const;
		Matrix & operator*=(const Matrix & mat);

		void x(float value);
		void y(float value);
		void z(float value);
		float  x() const;
		float  y() const;
		float  z() const;
		float  w() const;

		// ���g�̍s���ݒ肷��B���g�̍s��ɂ���Ɋ|�����킹��킯�ł͂Ȃ� **************

		Matrix & identity();
		Matrix & parallel(float vx, float vy, float vz);
		Matrix & scale(float magni_x, float magni_y, float magni_z);
		Matrix & rotate(float radian);			// rotate_z�Ɠ���
		Matrix & rotate(float sin_, float cos_);	// rotate_z�Ɠ���
		Matrix & affine(const Matrix & scale, const Matrix & rotate, const Matrix & trans);

		Matrix & rotate_x(float radian);
		Matrix & rotate_y(float radian);
		Matrix & rotate_z(float radian);

		Matrix & rotate_x(float sin_, float cos_);
		Matrix & rotate_y(float sin_, float cos_);
		Matrix & rotate_z(float sin_, float cos_);

		// *******************************************************************************

		const D3DXMATRIX & dxmat() const;
		const D3DXMATRIX * dxpointer() const;

	private:
		D3DXMATRIX matrix_;


	};
}