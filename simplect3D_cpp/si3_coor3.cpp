#include <si3_utility.h>

namespace si3
{
	Coor3::Coor3()
	{
		// Nothing
	}
	Coor3::Coor3(const D3DXVECTOR3 & dxcoor)
	{
		x = dxcoor.x;
		y = dxcoor.y;
		z = dxcoor.z;
	}
	Coor3::Coor3(const float x, const float y, const float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	bool Coor3::operator !=(const Coor3 & another) const
	{
		if (x != another.x) return true;
		if (y != another.y) return true;
		if (z != another.z) return true;

		return false;
	}

	Coor3 & Coor3::operator+=(const Coor3 & another)
	{
		x += another.x;
		y += another.y;
		z += another.z;

		return *this;
	}

	Coor3 Coor3::operator+(const Coor3 & another) const
	{
		Coor3 result;
		result.x = this->x + another.x;
		result.y = this->y + another.y;
		result.z = this->z + another.z;
		return result;
	}

	Coor3 Coor3::operator-(const Coor3 & another) const
	{
		Coor3 result;
		result.x = this->x - another.x;
		result.y = this->y - another.y;
		result.z = this->z - another.z;
		return result;
	}

	Coor3 Coor3::operator*(const float magni) const
	{
		Coor3 result;
		result.x = this->x * magni;
		result.y = this->y * magni;
		result.z = this->z * magni;
		return result;
	}


	float Coor3::length2() const
	{
		return x*x + y*y + z*z;
	}
	float Coor3::length() const
	{
		return sqrt(length2());
	}
	void Coor3::normalize()
	{
		const float magni = 1.0f / length();
		x = x*magni;
		y = y*magni;
		z = z*magni;
	}
	D3DXVECTOR3 Coor3::dxvec3() const
	{
		return D3DXVECTOR3(x, y, z);
	}


	float Coor3::dot(const si3::Coor3 & one, const si3::Coor3 & ano)
	{
		// todo
		D3DXVECTOR3 one_dx = { one.x, one.y, one.z };
		D3DXVECTOR3 ano_dx = { ano.x, ano.y, ano.z };
		return D3DXVec3Dot(&one_dx, &ano_dx);
	}

	si3::Coor3 Coor3::cross(const si3::Coor3 one, const si3::Coor3 & ano)
	{
		D3DXVECTOR3 one_dx = { one.x, one.y, one.z };
		D3DXVECTOR3 ano_dx = { ano.x, ano.y, ano.z };
		D3DXVECTOR3 result;
		D3DXVec3Cross(&result, &one_dx, &ano_dx);
		return si3::Coor3(result);
	}

}