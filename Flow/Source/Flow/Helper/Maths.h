#pragma once
#include <math.h>
#include <DirectXMath.h>

//Set the bit at the specified index to 1
#define BITMASK(Bit) (1 << Bit)

constexpr float PI = 3.14159265f;
constexpr double PI_D = 3.1415926535897932;

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
};

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
};