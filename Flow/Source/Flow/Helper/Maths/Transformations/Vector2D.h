#pragma once

struct Vector2D
{
	float X;
	float Y;

	Vector2D(float x, float y)
		: X(x), Y(y)
	{

	}

	Vector2D(float xy)
		: X(xy), Y(xy)
	{

	}

	bool operator==(const Vector2D& Other)
	{
		return (X == Other.X) && (Y == Other.Y);
	}

	bool operator!=(const Vector2D& Other)
	{
		return (X != Other.X) || (Y != Other.Y);
	}

	Vector2D operator-(const Vector2D& Other)
	{
		return Vector2D(X - Other.X, Y - Other.Y);
	}

	void operator-=(const Vector2D& Other)
	{
		X -= Other.X;
		Y -= Other.Y;
	}

	void operator*=(float Multiplier)
	{
		X *= Multiplier;
		Y *= Multiplier;
	}
};

struct IntVector2D
{
	int X;
	int Y;

	IntVector2D()
		: X(0), Y(0)
	{

	}

	IntVector2D(int x, int y)
		: X(x), Y(y)
	{

	}

	IntVector2D(int xy)
		: X(xy), Y(xy)
	{

	}

	bool operator==(const IntVector2D& Other)
	{
		return (X == Other.X) && (Y == Other.Y);
	}

	bool operator!=(const IntVector2D& Other)
	{
		return (X != Other.X) || (Y != Other.Y);
	}

	IntVector2D operator-(const IntVector2D& Other)
	{
		return IntVector2D(X - Other.X, Y - Other.Y);
	}

	void operator-=(const IntVector2D& Other)
	{
		X -= Other.X;
		Y -= Other.Y;
	}

	void operator*=(int Multiplier)
	{
		X *= Multiplier;
		Y *= Multiplier;
	}

	void operator+=(const IntVector2D& Other)
	{
		X += Other.X;
		Y += Other.Y;
	}
};