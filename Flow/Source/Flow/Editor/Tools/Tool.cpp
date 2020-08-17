#include "Flowpch.h"
#include "Tool.h"

Tool::Tool()
{
}

Tool::~Tool()
{
}

void Tool::UpdateTool(float DeltaTime)
{
}

void Tool::RenderTool()
{
}

void Tool::BeginPlay()
{
}

bool Tool::OnMouseButtonPressed(MouseButtonPressedEvent& e)
{
	return false;
}

bool Tool::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
{
	return false;
}

bool Tool::OnMouseMoved(MouseMovedEvent& e)
{
	return false;
}

bool Tool::OnMouseScrolled(MouseScrolledEvent& e)
{
	return false;
}

bool Tool::OnKeyPressed(KeyPressedEvent& e)
{
	return false;
}

bool Tool::OnKeyTyped(KeyTypedEvent& e)
{
	return false;
}

bool Tool::OnKeyReleased(KeyReleasedEvent& e)
{
	return false;
}

bool Tool::OnWindowResized(WindowResizedEvent& e)
{
	return false;
}

void Tool::OpenConfigWindow()
{
	m_ConfigurationWindowOpen = true;
}
