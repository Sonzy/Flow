#include "Flowpch.h"
#include "SkyboxActor.h"
#include "Flow/GameFramework/Components/SkyboxComponent.h"

SkyboxActor::SkyboxActor(const std::string& Name)
	: Actor(Name)
{
	_Skybox = CreateComponent<SkyboxComponent>("Skybox");
	_RootComponent = _Skybox.get();
}