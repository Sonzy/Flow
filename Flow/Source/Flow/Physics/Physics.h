#pragma once
#include "btBulletDynamicsCommon.h"
#include "Flow/Helper/Maths.h"

namespace Physics
{
	typedef btCollisionWorld::ClosestRayResultCallback TraceResult;

	TraceResult RayTrace(Vector Start, Vector End);
}