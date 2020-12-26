#include "Flowpch.h"
#include "SkyboxActor.h"
#include "Flow/GameFramework/Components/SkyboxComponent.h"

SkyboxActor::SkyboxActor()
	: SkyboxActor("Skybox Actor")
{
}

SkyboxActor::SkyboxActor(const std::string& Name)
	: Actor(Name)
{
	m_Skybox = CreateComponent<SkyboxComponent>("Skybox");
	m_Skybox->SetOwned(true);
	m_RootComponent = m_Skybox;
}