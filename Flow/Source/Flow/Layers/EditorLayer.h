#pragma once
#include "Flow/Layers/Layer.h"
#include "Flow/Events/MouseEvent.h"

namespace Flow
{
	class Inspector;
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();

		/* Layer interface */

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;

	protected:

		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		Inspector* m_Inspector;		
	};
}