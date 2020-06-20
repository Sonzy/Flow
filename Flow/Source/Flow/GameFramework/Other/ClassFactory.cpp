#include "Flowpch.h"
#include "Flow/Application.h"

#include "Flow/GameFramework/Actor.h"
#include "Flow/GameFramework/Components/WorldComponent.h"
#include "Flow/GameFramework/Components/StaticMeshComponent.h"
#include "Flow/GameFramework/Components/SkyboxComponent.h"


ClassFactory::ClassFactory()
{

}

void ClassFactory::RegisterClassUIDs()
{
	ClassFactory::Get().RegisterFactoryClass<Actor>();
	ClassFactory::Get().RegisterFactoryClass<WorldComponent>();
	ClassFactory::Get().RegisterFactoryClass<StaticMeshComponent>();
	ClassFactory::Get().RegisterFactoryClass<SkyboxComponent>();
}

ClassFactory& ClassFactory::Get()
{
	return Application::GetApplication().GetClassFactory();
}
