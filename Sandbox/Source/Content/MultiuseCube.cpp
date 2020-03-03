#include "MultiuseCube.h"
#include "Flow\GameFramework\Components\StaticMeshComponent.h"
#include "Flow\Assets\Materials\Mat_FlatColour.h"

MultiuseCube::MultiuseCube()
{
	//TODO: Get rid
	Flow::Material* TestMaterial = new Flow::Mat_FlatColour();

	m_StaticMesh = CreateComponent<Flow::StaticMeshComponent>("Cube");
	m_StaticMesh->InitialiseComponent("Box", TestMaterial);

	m_RootComponent = m_StaticMesh.get();
}