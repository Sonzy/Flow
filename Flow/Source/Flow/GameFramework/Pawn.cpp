#include "Flowpch.h"
#include "Pawn.h"
#include "Flow\Utils\HelperMacros.h"

Pawn::Pawn()
	: Pawn("Pawn")
{
}

Pawn::Pawn(const std::string& Name)
	: Actor(Name)
{
}

void Pawn::Control(Controller* NewController)
{
	CHECK_RETURN(!NewController, "Pawn::Control: Controller was nullptr");
	_Controller = NewController;

	OnControlled(_Controller);
}

void Pawn::RemoveControl()
{
	_Controller = nullptr;
}

void Pawn::OnControlled(Controller* OwningController)
{
}

Controller* Pawn::GetController() const
{
	return _Controller;
}