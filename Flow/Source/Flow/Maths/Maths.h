#pragma once

//= Includes ===================

#include "Typedefs.h"

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

#include "IntVector2.h"
#include "IntVector3.h"
#include "IntVector4.h"

#include "Rotator.h"
#include "Transform.h"

#define USE_DXMATH_FORMULAE 1

#if USE_DXMATH_FORMULAE
	#include <DirectXMath.h>
#endif

// Type Definitions //////////////////////////////////////////////

namespace Maths
{
	typedef DirectX::XMMATRIX Matrix;
}

// Forward Declarations ///////////////////////////////////////////

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

	Rotator			DegreesToRadians(const Rotator& Source);
	float			DegreesToRadians(float Source);
	Rotator			RadiansToDegrees(const Rotator& Source);
	float			RadiansToDegrees(float Source);

	/* Returns scales along each line for closest point between them */
	Vector3			GetClosestDistanceBetweenLines(const Physics::Ray& R1, const Physics::Ray& R2, float& OutR1Scale, float& OutR2Scale);

	float			Distance(Vector3 V1, Vector3 V2);
	float			DistanceSquared(Vector3 V1, Vector3 V2);

	Rotator			FindLookAtRotation(Vector3 StartPosition, Vector3 EndPosition);

#if USE_DXMATH_FORMULAE
	void			DecomposeMatrix(Vector3& translation, Rotator& rotation, Vector3& scale, DirectX::XMMATRIX matrix);

	void			PrintMatrix(DirectX::XMMATRIX m);
#endif

	void			PrintMatrix(float* m);
}