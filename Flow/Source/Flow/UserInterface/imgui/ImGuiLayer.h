#pragma once

#include "Flow/Layers/Layer.h"

#include "Flow/Events/KeyEvent.h"
#include "Flow/Events/MouseEvent.h"
#include "Flow/Events/ApplicationEvent.h"

namespace Flow
{
	class FLOW_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
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
	};
}