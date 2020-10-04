#pragma once

//= Includes ============================================

#include "Flow/GameFramework/Actor.h"

//= Forward Declarations ================================

class PointLightComponent;

//= Class Definitions ===================================

class FLOW_API PointLightActor : public Actor
{
public:

	//= Public Functions =====================

	PointLightActor();
	PointLightActor(const std::string& Name);

protected:

	//= Protected Variables ==================

	std::shared_ptr<PointLightComponent> m_PointLight;
};