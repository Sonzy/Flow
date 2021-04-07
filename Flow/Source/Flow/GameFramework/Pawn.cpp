#include "pch.h"
#include "Pawn.h"
#include "Framework\Utils\HelperMacros.h"

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
	m_Controller = NewController;

	OnControlled(m_Controller);
}

void Pawn::RemoveControl()
{
	m_Controller = nullptr;
}

void Pawn::OnControlled(Controller* OwningController)
{
}

Controller* Pawn::GetController() const
{
	return m_Controller;
}