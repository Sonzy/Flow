#pragma once

//= Includes ===================

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

#include "IntVector2.h"
#include "IntVector3.h"
#include "IntVector4.h"

#include "Rotator.h"
#include "Transform.h"

//= Forward Declarations =======

namespace Physics { class Ray; }

//= Namespace ==================

namespace Maths
{
	constexpr float PI = 3.14159265f;
	constexpr double PI_D = 3.1415926535897932;

	//= Template Functions ==================

	template<typename T>
	static T WrapAngle(T Theta)
	{
		const T modded = fmod(Theta, (T)2.0 * (T)PI_D);
		return (modded > (T)PI_D) ?
			(modded - (T)2.0 * (T)PI_D) :
			modded;
	}

	//= Functions ===========================

	Rotator DegreesToRadians(const Rotator& Source);
	float DegreesToRadians(float Source);

	/* Returns scales along each line for closest point between them */
	Vector3 GetClosestDistanceBetweenLines(const Physics::Ray& R1, const Physics::Ray& R2, float& OutR1Scale, float& OutR2Scale);


}