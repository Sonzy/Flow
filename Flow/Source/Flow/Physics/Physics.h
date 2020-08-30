#pragma once
#include "btBulletDynamicsCommon.h"
#include "Flow/Helper/Maths.h"

namespace Physics
{
	typedef btCollisionWorld::ClosestRayResultCallback TraceResult;
	typedef btCollisionWorld::AllHitsRayResultCallback TraceResultMulti;

	TraceResult RayTrace(Vector Start, Vector End);
	TraceResultMulti RayTraceMulti(Vector Start, Vector End);
}