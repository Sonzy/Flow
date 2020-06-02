#include "Flowpch.h"
#include "CollisionEditor.h"
#include "Flow/Application.h"

CollisionEditor::CollisionEditor()
{
	_Window = Window::Create(WindowProperties("Collision Editor", 1280u, 720u), false);
	_Window->SetEventCallback(std::bind(&CollisionEditor::OnEvent, this, std::placeholders::_1));
	Application::RegisterWindow(_Window);
}

void CollisionEditor::OnEvent(Event& e)
{
	e._Handled = true;

	EventDispatcher Dispatcher(e);
	Dispatcher.Dispatch<WindowClosedEvent>(std::bind(&CollisionEditor::OnWindowClosed, this, std::placeholders::_1));
}

bool CollisionEditor::OnWindowClosed(WindowClosedEvent& e)
{
	_Window->Shutdown();
	return true;
}
