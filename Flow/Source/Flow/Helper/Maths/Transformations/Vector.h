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

	static float Dot(const Vector& A, const Vector& B)
	{
		return (A.X * B.X) + (A.Y * B.Y) + (A.Z * B.Z);
	}

	static Vector Cross(const Vector& A, const Vector& B)
	{
		return Vector((A.Y * B.Z) - (A.Z * B.Y), (A.Z * B.X) - (A.X * B.Z) , (A.X * B.Y) - (A.Y * B.X) );
	}

	Vector Normalize()
	{
		float VecLength = Length();

		return VecLength > 0 ?
			Vector(X / VecLength, Y / VecLength, Z / VecLength) :
			*this;	
	}

	float Length()
	{
		return sqrt((X * X) + (Y * Y) + (Z * Z));
	}

	void operator+=(const Vector& Other)
	{
		X += Other.X;
		Y += Other.Y;
		Z += Other.Z;
	}

	Vector operator+(const Vector& Other) const
	{
		return Vector(X + Other.X, Y + Other.Y, Z + Other.Z);
	}

	Vector operator-(const Vector& Other) const
	{
		return Vector(X - Other.X, Y - Other.Y, Z - Other.Z);
	}

	Vector operator-() const
	{
		return Vector(-X, -Y, -Z);
	}

	Vector operator*(const Vector& Other) const
	{
		return Vector(X * Other.X, Y * Other.Y, Z * Other.Z);
	}

	Vector operator*(const float& Other) const
	{
		return Vector(X * Other, Y * Other, Z * Other);
	}

	void operator*=(const Vector& Other)
	{
		X *= Other.X;
		Y *= Other.Y;
		Z *= Other.Z;
	}

	template<typename OStream>
	friend OStream& operator<<(OStream& os, const Vector& c)
	{
		return os << c.X << "," << c.Y << "," << c.Z;
	}
};