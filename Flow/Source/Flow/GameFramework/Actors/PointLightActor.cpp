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
	m_PointLight = CreateComponent<PointLightComponent>("Point Light");
	m_RootComponent = m_PointLight;
}