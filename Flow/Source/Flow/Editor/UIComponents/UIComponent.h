#pragma once

// Includes ///////////////////////////////

// Forward Declarations ///////////////////

class MouseButtonPressedEvent;
class MouseButtonReleasedEvent;
class MouseMovedEvent;
class MouseScrolledEvent;
class KeyPressedEvent;
class KeyReleasedEvent;
class WindowResizedEvent;
class KeyTypedEvent;
class Editor;

// Class Definition ///////////////////////

class UIComponent
{
public:

	// Public Functions ///////////////////////////

							UIComponent();
	virtual					~UIComponent();

	virtual void			Update() = 0;
	virtual void			Render() = 0;

	virtual void			BeginPlay() {};

	//= Events =

	virtual bool			OnMouseButtonPressed(MouseButtonPressedEvent& e);
	virtual bool			OnMouseButtonReleased(MouseButtonReleasedEvent& e);
	virtual bool			OnMouseMoved(MouseMovedEvent& e);
	virtual bool			OnMouseScrolled(MouseScrolledEvent& e);
	virtual bool			OnKeyPressed(KeyPressedEvent& e);
	virtual bool			OnKeyTyped(KeyTypedEvent& e);
	virtual bool			OnKeyReleased(KeyReleasedEvent& e);
	virtual bool			OnWindowResized(WindowResizedEvent& e);

protected:
	friend class Editor;

	Editor*					m_Editor;
};