#include "Flowpch.h"
#include "Flow/Application.h"

#include "Flow/GameFramework/Actor.h"
#include "Flow/GameFramework/Components/WorldComponent.h"
#include "Flow/GameFramework/Components/StaticMeshComponent.h"
#include "Flow/GameFramework/Components/SkyboxComponent.h"
#include "Flow/GameFramework/Components/Lights/PointLightComponent.h"
#include "Flow/GameFramework/Components/Lights/DirectionalLightComponent.h"
#include "Flow/GameFramework/Components/SpriteComponent.h"

#include "Flow/GameFramework/Actors/StaticMeshActor.h"
#include "Flow/GameFramework/Actors/PointLightActor.h"
#include "Flow/GameFramework/Actors/DirectionalLightActor.h"
#include "Flow/GameFramework/Actors/SkyboxActor.h"
#include "Flow/GameFramework/Actors/CameraActor.h"
#include "Flow/GameFramework/Actors/SpriteActor.h"

ClassFactory::ClassFactory()
{

}

void ClassFactory::RegisterClassUIDs()
{
	ClassFactory::Get().RegisterFactoryClass<Actor>();
	ClassFactory::Get().RegisterFactoryClass<WorldComponent>();
	ClassFactory::Get().RegisterFactoryClass<StaticMeshComponent>();
	ClassFactory::Get().RegisterFactoryClass<SkyboxComponent>();
	ClassFactory::Get().RegisterFactoryClass<PointLightComponent>();
	ClassFactory::Get().RegisterFactoryClass<DirectionalLightComponent>();
	ClassFactory::Get().RegisterFactoryClass<SpriteComponent>();

	ClassFactory::Get().RegisterFactoryClass<StaticMeshActor>();
	ClassFactory::Get().RegisterFactoryClass<PointLightActor>();
	ClassFactory::Get().RegisterFactoryClass<DirectionalLightActor>();
	ClassFactory::Get().RegisterFactoryClass<SkyboxActor>();
	ClassFactory::Get().RegisterFactoryClass<CameraActor>();
	ClassFactory::Get().RegisterFactoryClass<SpriteActor>();
}

ClassFactory& ClassFactory::Get()
{
	return Application::Get().GetClassFactory();
}
