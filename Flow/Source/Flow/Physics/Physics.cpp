#include "Flowpch.h"
#include "Physics.h"

#include "Flow\Application.h"
#include "Flow/GameFramework/World.h"

Physics::TraceResult Physics::RayTrace(Vector Start, Vector End)
{
	btVector3 InternalStart = btVector3(Start.X, Start.Y, Start.Z);
	btVector3 InternaEnd = btVector3(End.X, End.Y, End.Z);

	btCollisionWorld::ClosestRayResultCallback Result(InternalStart, InternaEnd);
	Application::GetWorld()->GetPhysicsWorld()->rayTest(InternalStart, InternaEnd, Result);
	return Result;
}
