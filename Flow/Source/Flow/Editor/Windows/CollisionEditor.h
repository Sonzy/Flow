#pragma once
#include "Flow/Window/Window.h"
#include "Flow/Events/Event.h"
#include "Flow/Events/ApplicationEvent.h"
#include "EditorWindow.h"

class CollisionEditor : public EditorWindow
{
public:
	CollisionEditor();

	void OnEvent(Event& e);
	bool OnWindowClosed(WindowClosedEvent& e);


	Window* _Window;
};