#pragma once

// Includes /////////////////////////////////////////////////

#include "Tool.h"
#include "Maths/Maths.h"

#include "ThirdParty/ImGuizmo/ImGuizmo.h"

#include "Rendering/Core/Bindables/Texture.h"

// Forward Declaration //////////////////////////////////////

class WorldComponent;
class Actor;
class SelectionGizmo;
class Inspector;

// Public Enums /////////////////////////////////////////////

enum class TransformMode
{
	Translate,
	Rotate,
	Scale
};

// Class Definition /////////////////////////////////////////

class SelectionTool : public Tool
{
public:
	
	// Public Functions ///////////////////////////////////////////////////////////

												SelectionTool();
	virtual										~SelectionTool();

												SelectionTool(SelectionTool&) = delete;
												SelectionTool(SelectionTool&&) = delete;

	virtual void								UpdateTool(float DeltaTime) override;
	virtual void								RenderTool() override;
	virtual void								BeginPlay() override;

	void										RenderImGuiGizmo();

	virtual bool								OnMouseButtonPressed(MouseButtonPressedEvent& e) override;
	virtual bool								OnKeyPressed(KeyPressedEvent& e) override;
	virtual bool								OnMouseButtonReleased(MouseButtonReleasedEvent& e);

	virtual void								DrawConfigWindow() override;

	void										SelectComponent(WorldComponent* NewComponent);

	WorldComponent* const						GetSelectedComponent() { return m_SelectedComponent; }

	static ImGuizmo::OPERATION					TranslateTransformation(TransformMode mode);

private:
	
	// Private Functions ///////////////////////////////////////////////////////////

	// Private Variables ///////////////////////////////////////////////////////////

	WorldComponent*								m_SelectedComponent;
	Actor*										m_SelectedActor;
	TransformMode								m_transformationMode;

	SelectionGizmo*								m_Gizmo;


	//TEST
	Matrix4x4									m_CurrentMatrix;

	//TODO: remove -> Picking testing
	Microsoft::WRL::ComPtr<ID3D11Texture2D>		pNewTexture;
	Vector4										recentcolorclicked;


	//= Settings =

	bool										m_DrawSelectionLines;
	std::vector<std::pair<Vector3, Vector3>>	m_PreviousLines;
	Vector3										m_DebugLineColor;
	ImGuizmo::MODE								m_SpaceMode;//TODO: rename

};