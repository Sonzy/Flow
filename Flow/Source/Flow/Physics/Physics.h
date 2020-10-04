#pragma once

//= Includes =================================

#include "Bullet/btBulletDynamicsCommon.h"
#include "Maths/Vector3.h"

namespace Physics
{
	//= Type Definitions ===========================

	typedef btCollisionWorld::ClosestRayResultCallback TraceResult;
	typedef btCollisionWorld::AllHitsRayResultCallback TraceResultMulti;

	//= Class Definitions ==========================

	class Ray
	{
	public:

		//= Public Functions ============

		Ray(Vector3 Origin, Vector3 Direction)
			: m_Origin(Origin), m_Direction(Direction)
		{}

	public:

		//= Public Variables ============

		Vector3 m_Origin;
		Vector3 m_Direction;
	};

	//= Namespace Functions =======================

	TraceResult RayTrace(Vector3 Start, Vector3 End);
	TraceResultMulti RayTraceMulti(Vector3 Start, Vector3 End);
}