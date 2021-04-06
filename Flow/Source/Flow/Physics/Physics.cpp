#include "pch.h"

//= Includes ================

#include "Application.h"
#include "Physics.h"
#include "GameFramework/World.h"

//= Function Definitions =======================

Physics::TraceResult Physics::RayTrace(Vector3 Start, Vector3 End)
{
	btCollisionWorld::ClosestRayResultCallback Result(Start, End);
	Application::GetWorld()->GetPhysicsWorld()->rayTest(Start, End, Result);

	return Result;
}

Physics::TraceResultMulti Physics::RayTraceMulti(Vector3 Start, Vector3 End)
{
	btCollisionWorld::AllHitsRayResultCallback Result(Start, End);
	Application::GetWorld()->GetPhysicsWorld()->rayTest(Start, End, Result);

	return Result;
}
