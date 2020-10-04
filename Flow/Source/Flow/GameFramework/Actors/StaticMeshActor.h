#pragma once

//= Includes ================================================

#include "Flow/GameFramework/Actor.h"

//= Forward Declarations ====================================

class StaticMeshComponent;

//= Class Definitions =======================================

class FLOW_API StaticMeshActor : public Actor
{
public:
	
	//= Public Functions =======================

											StaticMeshActor();
											StaticMeshActor(const std::string& NewName);
	virtual									~StaticMeshActor();

	StaticMeshComponent*					GetMeshComponent() const;

protected:

	//= Protected Variables ====================

	std::shared_ptr<StaticMeshComponent>	m_MeshComponent;
};