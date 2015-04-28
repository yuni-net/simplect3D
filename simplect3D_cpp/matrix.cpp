#include <matrix.h>
#include <assert.h>

namespace si3
{
	D3DXMATRIX inverse_matrix_of_rot(const D3DXMATRIX & rot)
	{
		D3DXMATRIX inverse = rot;

		inverse._12 = rot._21;
		inverse._21 = rot._12;

		inverse._13 = rot._31;
		inverse._31 = rot._13;

		inverse._23 = rot._32;
		inverse._32 = rot._23;

		return inverse;
	}

	matrix::matrix()
	{
		matrix_(0, 0) = 1.0f;
		matrix_(0, 1) = 0.0f;
		matrix_(0, 2) = 0.0f;
		matrix_(0, 3) = 0.0f;

		matrix_(1, 0) = 0.0f;
		matrix_(1, 1) = 1.0f;
		matrix_(1, 2) = 0.0f;
		matrix_(1, 3) = 0.0f;

		matrix_(2, 0) = 0.0f;
		matrix_(2, 1) = 0.0f;
		matrix_(2, 2) = 1.0f;
		matrix_(2, 3) = 0.0f;

		matrix_(3, 0) = 0.0f;
		matrix_(3, 1) = 0.0f;
		matrix_(3, 2) = 0.0f;
		matrix_(3, 3) = 1.0f;
	}
	matrix::matrix(
		float _00, float _01, float _02,
		float _10, float _11, float _12,
		float _20, float _21, float _22)
	{
		matrix_(0, 0) = _00;
		matrix_(0, 1) = _01;
		matrix_(0, 2) = _02;
		matrix_(0, 3) = 0.0f;

		matrix_(1, 0) = _10;
		matrix_(1, 1) = _11;
		matrix_(1, 2) = _12;
		matrix_(1, 3) = 0.0f;

		matrix_(2, 0) = _20;
		matrix_(2, 1) = _21;
		matrix_(2, 2) = _22;
		matrix_(2, 3) = 0.0f;

		matrix_(3, 0) = 0.0f;
		matrix_(3, 1) = 0.0f;
		matrix_(3, 2) = 0.0f;
		matrix_(3, 3) = 1.0f;
	}
	matrix::matrix(const matrix & mat)
	{
		matrix_ = mat.matrix_;
	}
	matrix::matrix(const D3DXMATRIX & dxmat)
	{
		matrix_ = dxmat;
	}
	matrix & matrix::operator=(const matrix & mat)
	{
		matrix_ = mat.matrix_;
		return *this;
	}
	matrix & matrix::operator=(const D3DXMATRIX & dxmat)
	{
		matrix_ = dxmat;
		return *this;
	}
	float matrix::operator()(unsigned int y, unsigned int x)
	{
		assert(x <= 3 && y <= 3);
		return matrix_(y, x);
	}
	matrix matrix::operator*(const matrix & mat) const
	{
		D3DXMATRIX result9 = matrix_ * mat.matrix_;

		matrix result(
			result9(0, 0), result9(0, 1), result9(0, 2),
			result9(1, 0), result9(1, 1), result9(1, 2),
			result9(2, 0), result9(2, 1), result9(2, 2));

		result.x(result9(3, 0));
		result.y(result9(3, 1));
		result.z(result9(3, 2));

		return result;
	}
	matrix & matrix::operator*=(const matrix & mat)
	{
		matrix_ *= mat.matrix_;
		return *this;
	}




	matrix & matrix::parallel(float fVX, float fVY, float fVZ)
	{
		*this = matrix(
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f);

		x(fVX);
		y(fVY);
		z(fVZ);

		return *this;
	}
	matrix & matrix::scale(float fMagniX, float fMagniY, float fMagniZ)
	{
		*this = matrix(
			fMagniX, 0.0f, 0.0f,
			0.0f, fMagniY, 0.0f,
			0.0f, 0.0f, fMagniZ );

		return *this;
	}
	matrix & matrix::rotate(float fRadian)
	{
		return rotate_z(fRadian);
	}
	matrix & matrix::rotate(float fSin, float fCos)
	{
		return rotate_z(fSin, fCos);
	}
	matrix & matrix::affine(const matrix & scale, const matrix & rotate, const matrix & trans)
	{
		*this = scale*rotate*trans;
		return *this;
	}

	matrix & matrix::rotate_x(float fRadian)
	{
		return rotate_x(::sin(fRadian), ::cos(fRadian));
	}
	matrix & matrix::rotate_y(float fRadian)
	{
		return rotate_y(::sin(fRadian), ::cos(fRadian));
	}
	matrix & matrix::rotate_z(float fRadian)
	{
		return rotate_z(::sin(fRadian), ::cos(fRadian));
	}


	matrix & matrix::rotate_x(float fSin, float fCos)
	{
		*this = matrix(
			1.0f, 0.0f, 0.0f,
			0.0f, fCos, -fSin,
			0.0f, fSin, fCos);

		return *this;
	}
	matrix & matrix::rotate_y(float fSin, float fCos)
	{
		*this = matrix(
			fCos, 0.0f, fSin,
			0.0f, 1.0f, 0.0f,
			-fSin, 0.0f, fCos);

		return *this;
	}
	matrix & matrix::rotate_z(float fSin, float fCos)
	{
		*this = matrix(
			fCos, -fSin, 0.0f,
			fSin, fCos, 0.0f,
			0.0f, 0.0f, 1.0f);

		return *this;
	}


	void matrix::x(float fValue)
	{
		matrix_(3, 0) = fValue;
	}
	void matrix::y(float fValue)
	{
		matrix_(3, 1) = fValue;
	}
	void matrix::z(float fValue)
	{
		matrix_(3, 2) = fValue;
	}
	float  matrix::x() const
	{
		return matrix_(3, 0);
	}
	float  matrix::y() const
	{
		return matrix_(3, 1);
	}
	float  matrix::z() const
	{
		return matrix_(3, 2);
	}
	float matrix::w() const
	{
		return matrix_(3, 3);
	}


	const D3DXMATRIX & matrix::dxmat() const
	{
		return matrix_;
	}
	const D3DXMATRIX * matrix::dxpointer() const
	{
		return &dxmat();
	}


}