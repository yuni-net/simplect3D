#include <si3_Matrix.h>
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

	Matrix::Matrix()
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
	Matrix::Matrix(
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
	Matrix::Matrix(const Matrix & mat)
	{
		matrix_ = mat.matrix_;
	}
	Matrix::Matrix(const D3DXMATRIX & dxmat)
	{
		matrix_ = dxmat;
	}
	Matrix & Matrix::operator=(const Matrix & mat)
	{
		matrix_ = mat.matrix_;
		return *this;
	}
	Matrix & Matrix::operator=(const D3DXMATRIX & dxmat)
	{
		matrix_ = dxmat;
		return *this;
	}
	float Matrix::operator()(unsigned int y, unsigned int x)
	{
		assert(x <= 3 && y <= 3);
		return matrix_(y, x);
	}
	Matrix Matrix::operator*(const Matrix & mat) const
	{
		D3DXMATRIX result9 = matrix_ * mat.matrix_;

		Matrix result(
			result9(0, 0), result9(0, 1), result9(0, 2),
			result9(1, 0), result9(1, 1), result9(1, 2),
			result9(2, 0), result9(2, 1), result9(2, 2));

		result.x(result9(3, 0));
		result.y(result9(3, 1));
		result.z(result9(3, 2));

		return result;
	}
	Matrix & Matrix::operator*=(const Matrix & mat)
	{
		matrix_ *= mat.matrix_;
		return *this;
	}




	Matrix & Matrix::parallel(float vx, float vy, float vz)
	{
		*this = Matrix(
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f);

		x(vx);
		y(vy);
		z(vz);

		return *this;
	}
	Matrix & Matrix::scale(float magni_x, float magni_y, float magni_z)
	{
		*this = Matrix(
			magni_x, 0.0f, 0.0f,
			0.0f, magni_y, 0.0f,
			0.0f, 0.0f, magni_z);

		return *this;
	}
	Matrix & Matrix::rotate(float radian)
	{
		return rotate_z(radian);
	}
	Matrix & Matrix::rotate(float sin_, float cos_)
	{
		return rotate_z(sin_, cos_);
	}
	Matrix & Matrix::affine(const Matrix & scale, const Matrix & rotate, const Matrix & trans)
	{
		*this = scale*rotate*trans;
		return *this;
	}

	Matrix & Matrix::rotate_x(float radian)
	{
		return rotate_x(::sin(radian), ::cos(radian));
	}
	Matrix & Matrix::rotate_y(float radian)
	{
		return rotate_y(::sin(radian), ::cos(radian));
	}
	Matrix & Matrix::rotate_z(float radian)
	{
		return rotate_z(::sin(radian), ::cos(radian));
	}


	Matrix & Matrix::rotate_x(float sin_, float cos_)
	{
		*this = Matrix(
			1.0f, 0.0f, 0.0f,
			0.0f, cos_, -sin_,
			0.0f, sin_, cos_);

		return *this;
	}
	Matrix & Matrix::rotate_y(float sin_, float cos_)
	{
		*this = Matrix(
			cos_, 0.0f, sin_,
			0.0f, 1.0f, 0.0f,
			-sin_, 0.0f, cos_);

		return *this;
	}
	Matrix & Matrix::rotate_z(float sin_, float cos_)
	{
		*this = Matrix(
			cos_, -sin_, 0.0f,
			sin_, cos_, 0.0f,
			0.0f, 0.0f, 1.0f);

		return *this;
	}


	void Matrix::x(float value)
	{
		matrix_(3, 0) = value;
	}
	void Matrix::y(float value)
	{
		matrix_(3, 1) = value;
	}
	void Matrix::z(float value)
	{
		matrix_(3, 2) = value;
	}
	float  Matrix::x() const
	{
		return matrix_(3, 0);
	}
	float  Matrix::y() const
	{
		return matrix_(3, 1);
	}
	float  Matrix::z() const
	{
		return matrix_(3, 2);
	}
	float Matrix::w() const
	{
		return matrix_(3, 3);
	}


	const D3DXMATRIX & Matrix::dxmat() const
	{
		return matrix_;
	}
	const D3DXMATRIX * Matrix::dxpointer() const
	{
		return &dxmat();
	}


}