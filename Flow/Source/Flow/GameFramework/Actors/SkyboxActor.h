#pragma once

//= Includes ===========================================

#include "Flow/GameFramework/Actor.h"

//= Forward Declarations ===============================

class SkyboxComponent;

//= Class Definitions ==================================

class FLOW_API SkyboxActor : public Actor
{
public:

	REGISTER_CLASS(SkyboxActor)
	
	//= Public Functions ===========================

										SkyboxActor();
										SkyboxActor(const std::string& Name);

public:

	//= Public Variables ===========================
	
	SkyboxComponent*					m_Skybox;
};