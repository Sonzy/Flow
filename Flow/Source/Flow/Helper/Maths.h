#pragma once
#include <math.h>
#include "Maths\Transformations\Transform.h"
#include "Maths\Transformations\Vector2D.h"

//Set the bit at the specified index to 1
#define BITMASK(Bit) (1 << Bit)
#define CHECKBIT(Bit, Bitmask) (Bitmask & Bit)

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

	static float DegreesToRadians(float Degrees)
	{
		return Degrees * (Math::PI / 180.0f);
	}

public:
	static constexpr float PI = 3.14159265f;
	static constexpr double PI_D = 3.1415926535897932;
};