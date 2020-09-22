#pragma once

//= Includes ===========================================

#include <cmath>
#include "spdlog/fmt/ostr.h"
#include "Vector3.h"

//= Class Definition ===================================

class FLOW_API IntVector3
{
public:

	//= Constructors ==================================================

	IntVector3()
		: x(0), y(0), z(0)
	{}

	IntVector3(int x, int y, int z)
		: x(x), y(y), z(z)
	{}

	IntVector3(int xyz)
		: x(xyz), y(xyz), z(xyz)
	{}

	IntVector3(Vector3 Input)
		: x(static_cast<int>(Input.x)), y(static_cast<int>(Input.y)), z(static_cast<int>(Input.z))
	{}

	//= Public Static Functions ========================================

	float Length()
	{
		return sqrt(((float)x * (float)x) + ((float)y * (float)y) + ((float)z * (float)z));
	}

	//= Operators =========================================================

	operator DirectX::XMFLOAT3() const
	{
		return { static_cast<float>(x), static_cast<float>(y), static_cast<float>(z) };
	}

	operator btVector3() const
	{
		return btVector3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
	}

	void operator+=(const IntVector3& Other)
	{
		x += Other.x;
		y += Other.y;
		z += Other.z;
	}

	IntVector3 operator+(const IntVector3& Other) const
	{
		return IntVector3(x + Other.x, y + Other.y, z + Other.z);
	}

	IntVector3 operator-(const IntVector3& Other) const
	{
		return IntVector3(x - Other.x, y - Other.y, z - Other.z);
	}

	IntVector3 operator-() const
	{
		return IntVector3(-x, -y, -z);
	}

	IntVector3 operator*(const IntVector3& Other) const
	{
		return IntVector3(x * Other.x, y * Other.y, z * Other.z);
	}

	IntVector3 operator*(const int& Other) const
	{
		return IntVector3(x * Other, y * Other, z * Other);
	}

	void operator*=(const IntVector3& Other)
	{
		x *= Other.x;
		y *= Other.y;
		z *= Other.z;
	}

	template<typename OStream>
	friend OStream& operator<<(OStream& os, const IntVector3& c)
	{
		return os << c.x << "," << c.y << "," << c.z;
	}

	int x;
	int y;
	int z;
};

//= External Inline Functions =======================================

//FLOW_API Vector3::Vector3(IntVector3 v)
//	: x(static_cast<float>(v.x)), y(static_cast<float>(v.y)), z(static_cast<float>(v.z))
//{}