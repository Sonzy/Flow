#pragma once

//= Includes ========================================

#include "Flow\GameFramework\Actor.h"

//= Forward Declarations ============================

class Controller;

//= Class Definitions ===============================

class FLOW_API Pawn : public Actor
{
public:

	//= Public Functions ============================

						Pawn();
						Pawn(const std::string& Name);

	void				Control(Controller* NewController);
	void				RemoveControl();
	virtual void		OnControlled(Controller* OwningController);
	Controller*			GetController() const;

private:

	//= Private Variables ===========================

	Controller*			m_Controller;
};