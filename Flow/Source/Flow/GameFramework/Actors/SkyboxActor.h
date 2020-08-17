#pragma once
#include "Flow/GameFramework/Actor.h"

class SkyboxComponent;

class FLOW_API SkyboxActor : public Actor
{
public:
	SkyboxActor();
	SkyboxActor(const std::string& Name);
	
	std::shared_ptr<SkyboxComponent> _Skybox;
};