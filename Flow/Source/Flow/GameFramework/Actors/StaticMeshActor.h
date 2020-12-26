#pragma once

//= Includes ================================================

#include "Flow/GameFramework/Actor.h"

//= Forward Declarations ====================================

class StaticMeshComponent;

//= Class Definitions =======================================

class FLOW_API StaticMeshActor : public Actor
{
public:

	REGISTER_CLASS(StaticMeshActor)
	
	//= Public Functions =======================

											StaticMeshActor();
											StaticMeshActor(const std::string& NewName);
	virtual									~StaticMeshActor();

	StaticMeshComponent*					GetMeshComponent() const;

	//= Save/Load =

	virtual void							Serialize(YAML::Emitter& Archive) override;
	virtual void							Deserialize(YAML::Node& Archive) override;

protected:

	//= Protected Variables ====================

	StaticMeshComponent*					m_MeshComponent;
};