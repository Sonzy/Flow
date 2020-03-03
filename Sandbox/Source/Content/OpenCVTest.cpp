#include "OpenCVTest.h"
#include "Flow\GameFramework\Components\StaticMeshComponent.h"
#include "Flow\Assets\Materials\Mat_FlatColour.h"

OpenCVTest::OpenCVTest()
{
	//TODO: Get rid
	Flow::Material* TestMaterial = new Flow::Mat_FlatColour();

	m_StaticMesh = CreateComponent<Flow::StaticMeshComponent>("OpenCV RootMesh");
	m_StaticMesh->InitialiseComponent("Plane", TestMaterial);

	m_RootComponent = m_StaticMesh.get();
}