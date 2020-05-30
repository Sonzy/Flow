#pragma once
#include <math.h>
#include "Maths\Transformations\Transform.h"
#include "Maths\Transformations\Vector2D.h"

//Set the bit at the specified index to 1
#define BITMASK(Bit) (1 << Bit)
#define CHECKBIT(Bit, Bitmask) (Bitmask & Bit)

struct Ray
{
	Vector Origin;
	Vector Direction;

	Ray(Vector Origin, Vector Direction)
		: Origin(Origin), Direction(Direction)
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

	static float DegreesToRadians(float Degrees)
	{
		return Degrees * (Math::PI / 180.0f);
	}

	/* Returns scales along each line for closest point between them */
	static Vector GetClosestDistanceBetweenLines(const Ray& R1, const Ray& R2, float& OutR1Scale, float& OutR2Scale)
	{
		Vector Distance = R2.Origin - R1.Origin;
		const float v12 = Vector::Dot(R1.Direction, R1.Direction);
		const float v22 = Vector::Dot(R2.Direction, R2.Direction);
		const float v1v2 = Vector::Dot(R1.Direction, R2.Direction);

		const float det = v1v2 * v1v2 - v12 * v22;

		if (std::abs(det) > FLT_MIN)
		{
			const float inv_det = 1.f / det;

			const float dpv1 = Vector::Dot(Distance, R1.Direction);
			const float dpv2 = Vector::Dot(Distance, R2.Direction);

			OutR1Scale = inv_det * (v22 * dpv1 - v1v2 * dpv2);
			OutR2Scale = inv_det * (v1v2 * dpv1 - v12 * dpv2);

			return Vector(Distance + R2.Direction * OutR2Scale - R1.Direction * OutR1Scale).Normalize();
		}
		else
		{
			const Vector a = Vector::Cross(Distance, R1.Direction);
			return std::sqrt(Vector::Dot(a, a) / v12);
		}
	}

public:
	static constexpr float PI = 3.14159265f;
	static constexpr double PI_D = 3.1415926535897932;
};