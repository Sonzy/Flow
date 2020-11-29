//= Includes ==============================

#include "Flowpch.h"
#include "DebugDraw.h"
#include "GameFramework/World.h"

//= Function Definitions ==================

void DebugDraw::DrawLine(Vector3 Start, Vector3 End, Vector3 Color)
{
	World::GetLineBatcher_S().AddLine(Start, End, Color);
}

void DebugDraw::DrawBox(Vector3 Centre, float Size)
{
	//TODO: Make a box loool
	const float ExtentSize = Size / 2.0f;

	//Front Square
	World::GetLineBatcher_S().AddLine(Centre + Vector3(-ExtentSize, -ExtentSize, ExtentSize), Centre + Vector3(ExtentSize, -ExtentSize, ExtentSize));
	World::GetLineBatcher_S().AddLine(Centre + Vector3(-ExtentSize, -ExtentSize, ExtentSize), Centre + Vector3(-ExtentSize, ExtentSize, ExtentSize));
	World::GetLineBatcher_S().AddLine(Centre + Vector3(ExtentSize, -ExtentSize, ExtentSize), Centre + Vector3(ExtentSize, -ExtentSize, ExtentSize));
	World::GetLineBatcher_S().AddLine(Centre + Vector3(-ExtentSize, ExtentSize, ExtentSize), Centre + Vector3(ExtentSize, -ExtentSize, ExtentSize));

	//Back Square
	World::GetLineBatcher_S().AddLine(Centre + Vector3(-ExtentSize, -ExtentSize, -ExtentSize), Centre + Vector3(ExtentSize, -ExtentSize, -ExtentSize));
	World::GetLineBatcher_S().AddLine(Centre + Vector3(-ExtentSize, -ExtentSize, -ExtentSize), Centre + Vector3(-ExtentSize, ExtentSize, -ExtentSize));
	World::GetLineBatcher_S().AddLine(Centre + Vector3(ExtentSize, -ExtentSize, -ExtentSize), Centre + Vector3(ExtentSize, -ExtentSize, -ExtentSize));
	World::GetLineBatcher_S().AddLine(Centre + Vector3(-ExtentSize, ExtentSize, -ExtentSize), Centre + Vector3(ExtentSize, -ExtentSize, -ExtentSize));

	//Connecting Lines 
	World::GetLineBatcher_S().AddLine(Centre + Vector3(-ExtentSize, -ExtentSize, ExtentSize), Centre + Vector3(-ExtentSize, -ExtentSize, -ExtentSize));
	World::GetLineBatcher_S().AddLine(Centre + Vector3(ExtentSize, -ExtentSize, ExtentSize), Centre + Vector3(ExtentSize, -ExtentSize, -ExtentSize));
	World::GetLineBatcher_S().AddLine(Centre + Vector3(-ExtentSize, ExtentSize, ExtentSize), Centre + Vector3(-ExtentSize, ExtentSize, -ExtentSize));
	//World::GetLineBatcher_S().AddLine(Centre + Vector3(-ExtentSize, -ExtentSize, ExtentSize), Centre + Vector3(-ExtentSize, -ExtentSize, -ExtentSize));
}
