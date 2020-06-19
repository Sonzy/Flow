#include "Flowpch.h"
#include "PointLightActor.h"
#include "Flow/GameFramework/Components/PointLightComponent.h"

PointLightActor::PointLightActor()
	: PointLightActor("Unnamed Point Light Actor")
{
}

PointLightActor::PointLightActor(const std::string& Name)
	: Actor(Name)
{
	_PointLight = CreateComponent<PointLightComponent>("Point Light");
	_RootComponent = _PointLight.get();
}
