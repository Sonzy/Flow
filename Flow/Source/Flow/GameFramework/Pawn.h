#pragma once
#include "Flow\GameFramework\Actor.h"

class Controller;

class FLOW_API Pawn : public Actor
{
public:
	Pawn();
	Pawn(const std::string& Name);

	void Control(Controller* NewController);
	void RemoveControl();

	virtual void OnControlled(Controller* OwningController);

	Controller* GetController() const;

private:

	Controller* _Controller;
};