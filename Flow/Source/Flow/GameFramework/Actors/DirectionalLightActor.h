#pragma once

//= Includes ============================================

#include "Flow/GameFramework/Actor.h"

//= Forward Declarations ================================

class DirectionalLightComponent;

//= Class Definitions ===================================

class FLOW_API DirectionalLightActor : public Actor
{
public:

	REGISTER_CLASS(DirectionalLightActor)

		//= Public Functions =====================

	DirectionalLightActor();
	DirectionalLightActor(const std::string& Name);

	virtual void					Serialize(YAML::Emitter& Archive) override;
	virtual void					Deserialize(YAML::Node& Archive) override;

protected:

	//= Protected Variables ==================

	DirectionalLightComponent* m_DirectionalLight;
};