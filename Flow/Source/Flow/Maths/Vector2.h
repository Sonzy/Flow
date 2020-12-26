#pragma once

#include "spdlog/fmt/ostr.h"

class Vector3;
class IntVector2;

class FLOW_API Vector2
{
public:

	//= Constructors ==================================================

	Vector2()
		: x(0), y(0)
	{}

	Vector2(float x, float y)
		: x(x), y(y)
	{}

	Vector2(float xy)
		: x(xy), y(xy)
	{}

	Vector2(Vector3 v);

	Vector2(IntVector2 v);

	Vector2(DirectX::XMFLOAT2 Input)
		: x(Input.x), y(Input.y)
	{}

	float* Data()
	{
		return reinterpret_cast<float*>(this);
	}

	//= Operator ====================================================
	   
	Vector2 operator+(const Vector2 Other) const
	{
		return Vector2(x + Other.x, y + Other.y);
	}

	Vector2 operator-(const Vector2 Other) const
	{
		return Vector2(x - Other.x, y - Other.y);
	}

	Vector2 operator-() const
	{
		return Vector2(-x, -y);
	}

	Vector2 operator*(const float& Other) const
	{
		return Vector2(x * Other, y * Other);
	}

	Vector2 operator*(const Vector2& Other) const
	{
		return Vector2(x * Other.x, y * Other.y);
	}

	bool operator==(const Vector2& Other) const
	{
		return x == Other.x && y == Other.y;
	}

	bool operator!=(const Vector2& Other) const
	{
		return !operator==(Other);
	}

	template<typename OStream>
	friend OStream& operator<<(OStream& os, const Vector2& c)
	{
		return os << c.x << "," << c.y;
	}

	//= Public Variables ============================================

	float x;
	float y;
};