#pragma once

#include <cmath>
#include <DirectXMath.h>
#include "spdlog/fmt/ostr.h"
#include "ThirdParty/Bullet/LinearMath/btVector3.h"
#include "Vector2.h"

class Vector4;
class IntVector3;

class FLOW_API Vector3
{
public:

	//= Constructors ==================================================

	Vector3()
		: x(0), y(0), z(0)
	{}

	Vector3(float x, float y, float z)
		: x(x), y(y), z(z)
	{}

	Vector3(float xyz)
		: x(xyz), y(xyz), z(xyz)
	{}

	Vector3(Vector2 Input)
	{
		x = Input.x;
		y = Input.y;
	}

	Vector3(Vector4 Input);

	Vector3(IntVector3 Input);

	Vector3(IntVector2 Input);

	Vector3(btVector3 Input)
		: x(Input.x()), y(Input.y()), z(Input.z())
	{}

	//= Public Static Functions ========================================

	static float Dot(const Vector3& A, const Vector3& B)
	{
		return (A.x * B.x) + (A.y * B.y) + (A.z * B.z);
	}

	static Vector3 Cross(const Vector3& A, const Vector3& B)
	{
		return Vector3((A.y * B.z) - (A.z * B.y), (A.z * B.x) - (A.x * B.z), (A.x * B.y) - (A.y * B.x));
	}

	Vector3 Normalize()
	{
		float VecLength = Length();
		return VecLength > 0 ?	Vector3(x / VecLength, y / VecLength, z / VecLength) : *this;
	}

	float Length()
	{
		return sqrt((x * x) + (y * y) + (z * z));
	}

	//= Operators =========================================================

	operator DirectX::XMFLOAT3() const
	{
		return { x, y, z };
	}

	operator btVector3() const
	{
		return btVector3(x, y, z);
	}

	void operator+=(const Vector3& Other)
	{
		x += Other.x;
		y += Other.y;
		z += Other.z;
	}

	Vector3 operator+(const Vector3& Other) const
	{
		return Vector3(x + Other.x, y + Other.y, z + Other.z);
	}

	Vector3 operator-(const Vector3& Other) const
	{
		return Vector3(x - Other.x, y - Other.y, z - Other.z);
	}

	Vector3 operator-() const
	{
		return Vector3(-x, -y, -z);
	}

	Vector3 operator*(const Vector3& Other) const
	{
		return Vector3(x * Other.x, y * Other.y, z * Other.z);
	}

	Vector3 operator*(const float& Other) const
	{
		return Vector3(x * Other, y * Other, z * Other);
	}

	void operator*=(const Vector3& Other)
	{
		x *= Other.x;
		y *= Other.y;
		z *= Other.z;
	}

	template<typename OStream>
	friend OStream& operator<<(OStream& os, const Vector3& c)
	{
		return os << c.x << "," << c.y << "," << c.z;
	}

	float x;
	float y;
	float z;
};

//= External Inline Functions =======================================

//Vector2::Vector2(Vector3 v)
//	: x(v.x), y(v.y)
//{}