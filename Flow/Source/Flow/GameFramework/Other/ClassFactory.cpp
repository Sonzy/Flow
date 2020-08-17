#include "Flowpch.h"
#include "Flow/Application.h"

#include "Flow/GameFramework/Actor.h"
#include "Flow/GameFramework/Components/WorldComponent.h"
#include "Flow/GameFramework/Components/StaticMeshComponent.h"
#include "Flow/GameFramework/Components/SkyboxComponent.h"

#include "Flow/GameFramework/Actors/StaticMeshActor.h"
#include "Flow/GameFramework/Actors/PointLightActor.h"
#include "Flow/GameFramework/Actors/SkyboxActor.h"
#include "Flow/GameFramework/Actors/CameraActor.h"


ClassFactory::ClassFactory()
{

}

void ClassFactory::RegisterClassUIDs()
{
	ClassFactory::Get().RegisterFactoryClass<Actor>();
	ClassFactory::Get().RegisterFactoryClass<WorldComponent>();
	ClassFactory::Get().RegisterFactoryClass<StaticMeshComponent>();
	ClassFactory::Get().RegisterFactoryClass<SkyboxComponent>();

	ClassFactory::Get().RegisterFactoryClass<StaticMeshActor>();
	ClassFactory::Get().RegisterFactoryClass<PointLightActor>();
	ClassFactory::Get().RegisterFactoryClass<SkyboxActor>();
	ClassFactory::Get().RegisterFactoryClass<CameraActor>();
}

ClassFactory& ClassFactory::Get()
{
	return Application::GetApplication().GetClassFactory();
}
