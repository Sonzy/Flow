#include "ExampleWorldObject.h"

ExampleWorldObject::ExampleWorldObject()
{
	TestMesh = CreateComponent<Flow::StaticMeshComponent>();
	m_RootComponent = TestMesh.get();

	TestMesh->InitialiseComponent("Hat_FancyMan");
}
