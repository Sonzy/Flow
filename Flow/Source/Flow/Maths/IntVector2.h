#pragma once

//= Includes ===========================================

#include "spdlog/fmt/ostr.h"
#include "Vector2.h"
#include "Vector3.h"

//= Class Definition ===================================

class FLOW_API IntVector2
{
public:

	//= Constructors ==================================================

	IntVector2()
		: x(0), y(0)
	{}

	IntVector2(int x, int y)
		: x(x), y(y)
	{}

	IntVector2(float x, float y)
		: x(static_cast<int>(x)), y(static_cast<int>(y))
	{}

	IntVector2(int xy)
		: x(xy), y(xy)
	{}

	IntVector2(Vector2 v)
		: x(static_cast<int>(v.x)), y(static_cast<int>(v.y))
	{

	}

	//= Public Functions =============================================

	float Length()
	{
		return sqrt(((float)x * (float)x) + ((float)y * (float)y));
	}

	float Distance(const IntVector2& Other)
	{
		return (Other - *this).Length();
	}



	//= Operators ====================================================

	IntVector2 operator+(const IntVector2 Other) const
	{
		return IntVector2(x + Other.x, y + Other.y);
	}

	IntVector2 operator-(const IntVector2 Other) const
	{
		return IntVector2(x - Other.x, y - Other.y);
	}

	IntVector2 operator-() const 
	{
		return IntVector2(-x, -y);
	}

	IntVector2 operator*(const int& Other) const
	{
		return IntVector2(x * Other, y * Other);
	}

	IntVector2 operator*(const IntVector2& Other) const
	{
		return IntVector2(x * Other.x, y * Other.y);
	}

	bool operator==(const IntVector2& Other) const
	{
		return x == Other.x && y == Other.y;
	}

	bool operator!=(const IntVector2& Other) const
	{
		return !operator==(Other);
	}

	template<typename OStream>
	friend OStream& operator<<(OStream& os, const IntVector2& c)
	{
		return os << c.x << "," << c.y;
	}

	//= Public Variables ============================================

	int x;
	int y;
};

//FLOW_API Vector2::Vector2(IntVector2 v)
//	: x(static_cast<float>(v.x)), y(static_cast<float>(v.y))
//{
//}
//
//FLOW_API Vector3::Vector3(IntVector2 v)
//	: x(static_cast<float>(v.x)), y(static_cast<float>(v.y)), z(0)
//{
//}