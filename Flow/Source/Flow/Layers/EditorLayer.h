#pragma once
#include "Flow/Layers/Layer.h"
#include "Flow/Events/MouseEvent.h"
#include "Flow/Editor/Windows/EditorWindow.h"

class Inspector;
class Toolbar;
class SelectionGizmo;
class Application;
struct ImVec2;

class EditorLayer : public Layer
{
public:
	EditorLayer();

	/* Layer interface */

	virtual void BeginPlay() override;
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnImGuiRender(bool DrawEditor) override;
	void OnEvent(Event& e) override;

	virtual void OnUpdate(float DeltaTime) override;

	static EditorLayer* GetEditor();
	Inspector* GetInspector() const;
	Toolbar* GetToolbar() const;

	void SetDemoWindowVisible(bool Enabled);
	void ToggleImGuiDemoWindow();




	void OpenCollisionEditor();

protected:

	/* Create my own dockspace that takes into account the offset of the main menu bar.
	An almost-copy of the logic for creating a default dockstate.*/
	void InitialiseDockspace(ImVec2 Offset);
	void RenderApplicationDebug(float DeltaTime);

	bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
	bool OnMouseButtonReleased(MouseButtonReleasedEvent& e);

	void UpdateCollisionEditor();

	Inspector* _Inspector;
	Toolbar* _Toolbar;
	SelectionGizmo* _SelectionGizmo;
	bool _DrawDemoWindow = false;
	Application* _ApplicationPointer;

	std::vector<std::shared_ptr<EditorWindow>> _EditorWindows;


	//App Statistics
	float TimeSinceFrameUpdate = 0.0f;
	int FrameCounter = 0;
	float FrameTimer = 0.0f;
	float FrameDeltaTime = 0.0f;
	float LastFrameTime = 0.0f;
	const float UpdateInterval = 1.0f;


};