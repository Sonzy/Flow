#pragma once

#include "Vector.h"
#include "Vector2D.h"
#include <DirectXMath.h>
#include "spdlog/fmt/ostr.h"

struct Vector
{
	float X;
	float Y;
	float Z;

	Vector()
		: X(0), Y(0), Z(0)
	{}

	Vector(float x, float y, float z)
		: X(x), Y(y), Z(z)
	{}

	Vector(float xyz)
		: X(xyz), Y(xyz), Z(xyz)
	{}

	Vector(IntVector2D Input)
		: X((float)Input.X), Y((float)Input.Y), Z(0)
	{	}

	DirectX::XMFLOAT3 ToDXFloat3() const
	{
		return { X, Y, Z };
	}

	void operator+=(const Vector& Other)
	{
		X += Other.X;
		Y += Other.Y;
		Z += Other.Z;
	}

	Vector operator+(const Vector& Other)
	{
		return Vector(X + Other.X, Y + Other.Y, Z + Other.Z);
	}

	Vector operator-(const Vector& Other)
	{
		return Vector(X - Other.X, Y - Other.Y, Z - Other.Z);
	}

	Vector operator*(const float& Other)
	{
		return Vector(X * Other, Y * Other, Z * Other);
	}

	template<typename OStream>
	friend OStream& operator<<(OStream& os, const Vector& c)
	{
		return os << c.X << "," << c.Y << "," << c.Z;
	}
};