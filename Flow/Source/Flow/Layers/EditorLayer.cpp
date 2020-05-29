#include "Flowpch.h"
#include "EditorLayer.h"
#include "Flow/Events/MouseEvent.h"
#include "Flow\Editor\Inspector.h"
#include "Flow\Application.h"

#include "Flow\Editor\SelectionGizmo.h"

EditorLayer::EditorLayer()
	: Layer("Editor Layer")
{
}

void EditorLayer::OnAttach()
{
	_Inspector = Application::GetApplication().GetInspector();
}

void EditorLayer::OnDetach()
{
}

void EditorLayer::OnImGuiRender()
{
}

void EditorLayer::OnEvent(Event& e)
{
	EventDispatcher Dispatcher(e);
	Dispatcher.Dispatch<MouseButtonPressedEvent>(FLOW_BIND_EVENT_FUNCTION(EditorLayer::OnMouseButtonPressed));
}

void EditorLayer::OnUpdate(float DeltaTime)
{
	//m_Inspector->GetSelector()->Render();
}

bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
{
	if (_Inspector)
		return _Inspector->OnMouseClicked(e);

	return false;
}