#pragma once
#include "Flow/Layers/Layer.h"
#include "Flow/Events/MouseEvent.h"

class Inspector;
class SelectionGizmo;

class EditorLayer : public Layer
{
public:
	EditorLayer();

	/* Layer interface */

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnImGuiRender() override;
	void OnEvent(Event& e) override;

	virtual void OnUpdate(float DeltaTime) override;

protected:

	bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

	Inspector* _Inspector;
};