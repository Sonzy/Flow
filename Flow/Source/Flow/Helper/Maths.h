#pragma once
#include <math.h>
#include <DirectXMath.h>

//Set the bit at the specified index to 1
#define BITMASK(Bit) (1 << Bit)

class Math
{
public:

	template<typename T>
	static T WrapAngle(T Theta)
	{
		const T modded = fmod(Theta, (T)2.0 * (T)PI_D);
		return (modded > (T)PI_D) ?
			(modded - (T)2.0 * (T)PI_D) :
			modded;
	}

	static float DegreesToRadians(float Degrees);

public:
	static constexpr float PI = 3.14159265f;
	static constexpr double PI_D = 3.1415926535897932;
};

struct Vector
{
	float X;
	float Y;
	float Z;

	Vector(float x, float y, float z)
		: X(x), Y(y), Z(z)
	{

	}

	Vector(float xyz)
		: X(xyz), Y(xyz), Z(xyz)
	{

	}

	DirectX::XMFLOAT3 ToDXFloat3() const
	{
		return {X, Y, Z};
	}
};

struct Rotator
{
	float Pitch;
	float Roll;
	float Yaw;

	Rotator(float Pitch, float Roll, float Yaw)
		: Pitch(Pitch), Roll(Roll), Yaw(Yaw)
	{

	}

	Rotator(float PitchRollYaw)
		: Pitch(PitchRollYaw), Roll(PitchRollYaw), Yaw(PitchRollYaw)
	{

	}

	void operator+=(const Rotator & Other)
	{
		Pitch += Other.Pitch;
		Roll += Other.Roll;
		Yaw += Other.Yaw;

		//Lazy wrapping

		while (Pitch > 360.0f)
		{
			Pitch -= 360.0f;
		}
		while (Pitch < 0.0f)
		{
			Pitch += 360.0f;
		}

		while (Roll > 360.0f)
		{
			Roll -= 360.0f;
		}
		while (Roll < 0.0f)
		{
			Roll += 360.0f;
		}

		while (Yaw > 360.0f)
		{
			Yaw -= 360.0f;
		}
		while (Yaw < 0.0f)
		{
			Yaw += 360.0f;
		}
	}
};

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

	Vector2D operator-=(const Vector2D& Other)
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
	float Y;

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

	IntVector2D operator-=(const IntVector2D& Other)
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

