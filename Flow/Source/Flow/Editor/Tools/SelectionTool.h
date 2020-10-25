#pragma once
#include "Tool.h"

#include "Maths/Vector3.h"

class WorldComponent;
class Actor;
class SelectionGizmo;
class Inspector;

class SelectionTool : public Tool
{
public:

												SelectionTool();
	virtual										~SelectionTool();

												SelectionTool(SelectionTool&) = delete;
												SelectionTool(SelectionTool&&) = delete;

	virtual void								UpdateTool(float DeltaTime) override;
	virtual void								RenderTool() override;
	virtual void								BeginPlay() override;

	virtual bool								OnMouseButtonPressed(MouseButtonPressedEvent& e) override;
	virtual bool								OnKeyPressed(KeyPressedEvent& e) override;
	virtual bool								OnMouseButtonReleased(MouseButtonReleasedEvent& e);

	virtual void								DrawConfigWindow() override;

	void										SelectComponent(WorldComponent* NewComponent);

private:

	WorldComponent*								m_SelectedComponent;
	Actor*										m_SelectedActor;
	Inspector*									m_InspectorWindow;

	SelectionGizmo*								m_Gizmo;


	//= Settings =

	bool										m_DrawSelectionLines;
	std::vector<std::pair<Vector3, Vector3>>	m_PreviousLines;
	Vector3										m_DebugLineColor;

};