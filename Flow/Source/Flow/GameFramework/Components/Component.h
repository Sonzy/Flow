#pragma once

//= Includes =======================================================

#include "Flow\GameFramework\Gameobject.h"
#include <vector>

//= Forward Declarations ===========================================

class Actor;
class WorldComponent;

//= Class Definitions ==============================================

/* Base class for all components, contains attachment properties to a world object */
class FLOW_API Component : public GameObject
{
public:

	//= Public Functions ===============================

							Component();
							Component(const std::string& Name);
	virtual					~Component();

	WorldComponent*			GetParentComponent() const;
	Actor*					GetParentActor() const;

	void					SetParent(Actor* Parent);
	void					SetParentComponent(WorldComponent* Parent);

	virtual void			BeginPlay();
	virtual void			Tick(float DeltaTime) override;

	virtual void			DrawComponentDetailsWindow();

protected:

	//= Protected Variables ===========================

	Actor*					m_ParentObject;
	WorldComponent*			m_AttachedComponent;
};