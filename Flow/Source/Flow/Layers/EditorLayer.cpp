#include "Flowpch.h"
#include "EditorLayer.h"
#include "Flow/Events/MouseEvent.h"
#include "Flow\Editor\Inspector.h"
#include "Flow\Application.h"

namespace Flow
{
	EditorLayer::EditorLayer()
		: Layer("Editor Layer")
	{
	}

	void EditorLayer::OnAttach()
	{
		m_Inspector = Application::GetApplication().GetInspector();
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

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (m_Inspector)
			return m_Inspector->OnMouseClicked(e);

		return false;
	}
}

