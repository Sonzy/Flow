#pragma once
#include "Flow/GameFramework/Actor.h"

class PointLightComponent;

class FLOW_API PointLightActor : public Actor
{
public:
	PointLightActor();
	PointLightActor(const std::string& Name);

protected:

	std::shared_ptr<PointLightComponent> _PointLight;
};