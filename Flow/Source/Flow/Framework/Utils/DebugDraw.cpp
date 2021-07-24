//= Includes ==============================

#include "pch.h"
#include "DebugDraw.h"
#include "GameFramework/World.h"

//= Function Definitions ==================

void DebugDraw::DrawLine(Vector3 Start, Vector3 End, Vector3 Color)
{
	LineBatcher& batcher = World::Get().GetLineBatcher();
	batcher.AddLine(Start, End, Color);
}

void DebugDraw::DrawBox(Vector3 Centre, float Size)
{
	const float ExtentSize = Size / 2.0f;

	LineBatcher& batcher = World::Get().GetLineBatcher();

	//Front Square
	batcher.AddLine(Centre + Vector3(-ExtentSize, -ExtentSize, ExtentSize), Centre + Vector3(ExtentSize, -ExtentSize, ExtentSize));
	batcher.AddLine(Centre + Vector3(-ExtentSize, -ExtentSize, ExtentSize), Centre + Vector3(-ExtentSize, ExtentSize, ExtentSize));
	batcher.AddLine(Centre + Vector3(ExtentSize, -ExtentSize, ExtentSize), Centre + Vector3(ExtentSize, -ExtentSize, ExtentSize));
	batcher.AddLine(Centre + Vector3(-ExtentSize, ExtentSize, ExtentSize), Centre + Vector3(ExtentSize, -ExtentSize, ExtentSize));

	//Back Square
	batcher.AddLine(Centre + Vector3(-ExtentSize, -ExtentSize, -ExtentSize), Centre + Vector3(ExtentSize, -ExtentSize, -ExtentSize));
	batcher.AddLine(Centre + Vector3(-ExtentSize, -ExtentSize, -ExtentSize), Centre + Vector3(-ExtentSize, ExtentSize, -ExtentSize));
	batcher.AddLine(Centre + Vector3(ExtentSize, -ExtentSize, -ExtentSize), Centre + Vector3(ExtentSize, -ExtentSize, -ExtentSize));
	batcher.AddLine(Centre + Vector3(-ExtentSize, ExtentSize, -ExtentSize), Centre + Vector3(ExtentSize, -ExtentSize, -ExtentSize));

	//Connecting Lines 
	batcher.AddLine(Centre + Vector3(-ExtentSize, -ExtentSize, ExtentSize), Centre + Vector3(-ExtentSize, -ExtentSize, -ExtentSize));
	batcher.AddLine(Centre + Vector3(ExtentSize, -ExtentSize, ExtentSize), Centre + Vector3(ExtentSize, -ExtentSize, -ExtentSize));
	batcher.AddLine(Centre + Vector3(-ExtentSize, ExtentSize, ExtentSize), Centre + Vector3(-ExtentSize, ExtentSize, -ExtentSize));
	//batcher.AddLine(Centre + Vector3(-ExtentSize, -ExtentSize, ExtentSize), Centre + Vector3(-ExtentSize, -ExtentSize, -ExtentSize));
}
