#pragma once

// Typdefs /////////////////////////////////////

class MouseButtonPressedEvent;
class MouseButtonReleasedEvent;
class MouseMovedEvent;
class MouseScrolledEvent;
class KeyPressedEvent;
class KeyReleasedEvent;
class WindowResizedEvent;
class KeyTypedEvent;
class Editor;

// Class Definition /////////////////////////////

class Module
{
public:

	Module();
	~Module();

	virtual void Update() {};

	// Public Variables ///////////////////////////////////////////

	Editor* m_editor;
};