#include "Flowpch.h"
#include "CameraLayer.h"
#include "Flow/Rendering/RenderCommand.h"

#include "Flow/Input/KeyCodes.h"

namespace Flow
{
	CameraLayer::CameraLayer()
		: Layer("Camera Layer")
	{
	}

	CameraLayer::~CameraLayer()
	{
	}

	void CameraLayer::OnAttach()
	{
		m_CameraReference = &RenderCommand::GetCamera();
	}

	void CameraLayer::OnDetach()
	{
	}

	void CameraLayer::OnImGuiRender()
	{
		if(m_CameraReference)
			m_CameraReference->RenderIMGUIWindow();
	}

	void CameraLayer::OnEvent(Event& e)
	{
		EventDispatcher Dispatcher(e);
		Dispatcher.Dispatch<MouseButtonPressedEvent>(FLOW_BIND_EVENT_FUNCTION(CameraLayer::OnMouseButtonPressed));
		Dispatcher.Dispatch<MouseButtonReleasedEvent>(FLOW_BIND_EVENT_FUNCTION(CameraLayer::OnMouseButtonReleased));
		Dispatcher.Dispatch<MouseMovedEvent>(FLOW_BIND_EVENT_FUNCTION(CameraLayer::OnMouseMoved));
		Dispatcher.Dispatch<MouseScrolledEvent>(FLOW_BIND_EVENT_FUNCTION(CameraLayer::OnMouseScrolled));
		Dispatcher.Dispatch<KeyPressedEvent>(FLOW_BIND_EVENT_FUNCTION(CameraLayer::OnKeyPressed));
		Dispatcher.Dispatch<KeyReleasedEvent>(FLOW_BIND_EVENT_FUNCTION(CameraLayer::OnKeyReleased));
		Dispatcher.Dispatch<WindowResizedEvent>(FLOW_BIND_EVENT_FUNCTION(CameraLayer::OnWindowResized));
		Dispatcher.Dispatch<KeyTypedEvent>(FLOW_BIND_EVENT_FUNCTION(CameraLayer::OnKeyTyped));
	}

	bool CameraLayer::OnMouseButtonPressed(MouseButtonEvent& e)
	{
		return false;
	}

	bool CameraLayer::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
	{
		return false;
	}

	bool CameraLayer::OnMouseMoved(MouseMovedEvent& e)
	{
		return false;
	}

	bool CameraLayer::OnMouseScrolled(MouseScrolledEvent& e)
	{
		return false;
	}

	bool CameraLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		switch (e.GetKeyCode())
		{
		case FLOW_KEY_W:
			FLOW_ENGINE_LOG("Moving Camera W");
			m_CameraReference->Translate(DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f));
			break;
		case FLOW_KEY_A:
			FLOW_ENGINE_LOG("Moving Camera A");
			m_CameraReference->Translate(DirectX::XMFLOAT3( 1.0f, 0.0f, 0.0f ));
			break;
		case FLOW_KEY_S:
			FLOW_ENGINE_LOG("Moving Camera S");
			m_CameraReference->Translate(DirectX::XMFLOAT3( 0.0f, -1.0f, 0.0f ));
			break;
		case FLOW_KEY_D:
			FLOW_ENGINE_LOG("Moving Camera D");
			m_CameraReference->Translate(DirectX::XMFLOAT3 (-1.0f, 0.0f, 0.0f ));
			break;
		}
		return false;
	}

	bool CameraLayer::OnKeyTyped(KeyTypedEvent& e)
	{
		return false;
	}

	bool CameraLayer::OnKeyReleased(KeyReleasedEvent& e)
	{
		return false;
	}

	bool CameraLayer::OnWindowResized(WindowResizedEvent& e)
	{
		return false;
	}
}