#pragma once

//= Includes ================================================

#include "Flow/GameFramework/Actor.h"

//= Forward Declarations ====================================

class SpriteComponent;

//= Class Definitions =======================================

class FLOW_API SpriteActor : public Actor
{
public:

	//= Public Functions =======================

								SpriteActor();
								SpriteActor(const std::string& NewName);
	virtual						~SpriteActor();

	SpriteComponent*			GetSpriteComponent() const;

protected:

	//= Protected Variables ====================

	SpriteComponent*			m_SpriteComponent;
};