#pragma once

//= Includes ============================================

#include "Flow/GameFramework/Actor.h"

//= Forward Declarations ================================

class PointLightComponent;

//= Class Definitions ===================================

class FLOW_API PointLightActor : public Actor
{
public:

	REGISTER_CLASS(PointLightActor)

	//= Public Functions =====================

	PointLightActor();
	PointLightActor(const std::string& Name);

	virtual void					Serialize(YAML::Emitter& Archive) override;
	virtual void					Deserialize(YAML::Node& Archive) override;

protected:

	//= Protected Variables ==================

	PointLightComponent* m_PointLight;
};