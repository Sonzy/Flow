#include "Flowpch.h"
#include "UIComponent.h"

UIComponent::UIComponent()
{
}

UIComponent::~UIComponent()
{
}

bool UIComponent::OnMouseButtonPressed(MouseButtonPressedEvent& e)
{
	return false;
}

bool UIComponent::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
{
	return false;
}

bool UIComponent::OnMouseMoved(MouseMovedEvent& e)
{
	return false;
}

bool UIComponent::OnMouseScrolled(MouseScrolledEvent& e)
{
	return false;
}

bool UIComponent::OnKeyPressed(KeyPressedEvent& e)
{
	return false;
}

bool UIComponent::OnKeyTyped(KeyTypedEvent& e)
{
	return false;
}

bool UIComponent::OnKeyReleased(KeyReleasedEvent& e)
{
	return false;
}

bool UIComponent::OnWindowResized(WindowResizedEvent& e)
{
	return false;
}