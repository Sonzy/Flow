#pragma once
#include "Flow/Layers/Layer.h"
#include "Flow/Events/MouseEvent.h"
#include "Flow/Editor/Windows/EditorWindow.h"

class Inspector;
class Toolbar;
class SelectionGizmo;
class Application;
struct ImVec2;
class AssetWindow;
class EditorCamera;
class LevelManager;
class SpawnWindow;

class MouseButtonPressedEvent;
class MouseButtonReleasedEvent;
class MouseMovedEvent;
class MouseScrolledEvent;
class KeyPressedEvent;
class KeyReleasedEvent;
class WindowResizedEvent;
class KeyTypedEvent;

struct EditorSettings
{
	EditorSettings()
		:
		_ObjectHighlightColour(0.0f, 1.0f, 0.0f)
	{

	}


	Vector _ObjectHighlightColour;
};

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
	static EditorSettings& GetEditorSettings();
	Inspector* GetInspector() const;
	Toolbar* GetToolbar() const;

	void SetDemoWindowVisible(bool Enabled);
	void ToggleImGuiDemoWindow();



	bool IsSceneWindowFocused() const;
	bool IsMouseOverScene() const;

	IntVector2D GetSceneWindowSize() const;
	IntVector2D GetSceneWindowPosition() const;

	//Open Windows
	void Open_NewLevelWindow();
	void OpenCollisionEditor();
protected:

	/* Create my own dockspace that takes into account the offset of the main menu bar.
	An almost-copy of the logic for creating a default dockstate.*/
	void InitialiseDockspace(ImVec2 Offset);
	void RenderApplicationDebug(float DeltaTime);

	bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
	bool OnMouseButtonReleased(MouseButtonReleasedEvent& e);
	bool OnMouseMoved(MouseMovedEvent& e);
	bool OnMouseScrolled(MouseScrolledEvent& e);
	bool OnKeyPressed(KeyPressedEvent& e);
	bool OnKeyTyped(KeyTypedEvent& e);
	bool OnKeyReleased(KeyReleasedEvent& e);
	bool OnWindowResized(WindowResizedEvent& e);

	//Draw Windows
	void UpdateCollisionEditor();
	void DrawSceneWindow();

	Inspector* _Inspector;
	Toolbar* _Toolbar;
	SelectionGizmo* _SelectionGizmo;
	AssetWindow* _AssetWindow;
	bool _DrawDemoWindow = false;
	Application* _ApplicationPointer;
	EditorSettings _Settings;
	std::shared_ptr<EditorCamera> _EditorCam;

	bool _SceneWindowFocused;
	bool _MouseOverScene;
	IntVector2D _SceneWindowSize;
	IntVector2D _SceneWindowPosition;

	LevelManager* _LevelManager;

	SpawnWindow* _SpawnWindow;

	std::vector<std::shared_ptr<EditorWindow>> _EditorWindows;


	//App Statistics
	float TimeSinceFrameUpdate = 0.0f;
	int FrameCounter = 0;
	float FrameTimer = 0.0f;
	float FrameDeltaTime = 0.0f;
	float LastFrameTime = 0.0f;
	const float UpdateInterval = 1.0f;

	//Editor Window
	Vector2D _EditorViewportSize;
};