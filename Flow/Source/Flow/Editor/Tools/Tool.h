#pragma once

#include "ThirdParty/ImGui/imgui.h"

class MouseButtonPressedEvent;
class MouseButtonReleasedEvent;
class MouseMovedEvent;
class MouseScrolledEvent;
class KeyPressedEvent;
class KeyReleasedEvent;
class WindowResizedEvent;
class KeyTypedEvent;

class Tool
{
public:

							Tool();
	virtual					~Tool();

							Tool(Tool&) = delete;
							Tool(Tool&&) = delete;

	virtual void			UpdateTool(float DeltaTime);
	virtual void			RenderTool();
	virtual void			RenderToolImGui();
	virtual void			BeginPlay();

	virtual bool			OnMouseButtonPressed(MouseButtonPressedEvent& e);
	virtual bool			OnMouseButtonReleased(MouseButtonReleasedEvent& e);
	virtual bool			OnMouseMoved(MouseMovedEvent& e);
	virtual bool			OnMouseScrolled(MouseScrolledEvent& e);
	virtual bool			OnKeyPressed(KeyPressedEvent& e);
	virtual bool			OnKeyTyped(KeyTypedEvent& e);
	virtual bool			OnKeyReleased(KeyReleasedEvent& e);
	virtual bool			OnWindowResized(WindowResizedEvent& e);

	void					OpenConfigWindow();
	virtual void			DrawConfigWindow()			{}
	bool					IsConfigWindowOpen() const			{ return m_ConfigurationWindowOpen; }

protected:

	bool					m_ConfigurationWindowOpen;
};