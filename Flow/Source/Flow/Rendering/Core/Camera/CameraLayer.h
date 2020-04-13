#pragma once
#include "Flow/Layers/Layer.h"

#include "Flow/Events/KeyEvent.h"
#include "Flow/Events/MouseEvent.h"
#include "Flow/Events/ApplicationEvent.h"

#include "Flow/Rendering/Core/Camera/Camera.h"

//class Flow::Camera;

namespace Flow
{
	class CameraComponent;

	/* Temporary for testing camera movement */
	class FLOW_API CameraLayer : public Layer
	{
	public:
		CameraLayer();
		~CameraLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;

	private:

		bool OnMouseButtonPressed(MouseButtonEvent& e);
		bool OnMouseButtonReleased(MouseButtonReleasedEvent& e);
		bool OnMouseMoved(MouseMovedEvent& e);
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnKeyTyped(KeyTypedEvent& e);
		bool OnKeyReleased(KeyReleasedEvent& e);
		bool OnWindowResized(WindowResizedEvent& e);

		CameraComponent* CameraRef_;
	};
}