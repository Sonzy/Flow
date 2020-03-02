#include "OpenCVTest.h"
#include "Flow\GameFramework\Components\StaticMeshComponent.h"

OpenCVTest::OpenCVTest()
{
	m_StaticMesh = CreateComponent<Flow::StaticMeshComponent>();
	m_StaticMesh->InitialiseComponent("Plane");

	m_RootComponent = m_StaticMesh.get();
}
