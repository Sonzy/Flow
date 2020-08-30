#pragma once
#include "Flow/Layers/Layer.h"
#include "Flow/Events/MouseEvent.h"
#include "Flow/Editor/SceneManager.h"

class Inspector;
class MenuBar;
class ToolBar;
class Application;
struct ImVec2;
class AssetWindow;
class EditorCamera;
class LevelManager;
class SpawnWindow;
class Tool;
class Console;

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
	~EditorLayer();

	/* Layer interface */

	void Initialise();

	virtual void BeginPlay() override;
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnImGuiRender(bool DrawEditor) override;
	void OnEvent(Event& e) override;

	virtual void OnUpdate(float DeltaTime) override;

	static EditorLayer* GetEditor();
	static EditorSettings& GetEditorSettings();
	Inspector* GetInspector() const;
	MenuBar* GetMenuBar() const;

	void SetDemoWindowVisible(bool Enabled);
	void ToggleImGuiDemoWindow();



	bool IsSceneWindowFocused() const;
	bool IsMouseOverScene() const;

	IntVector2D GetSceneWindowSize() const;
	IntVector2D GetSceneWindowPosition() const;

	//Open Windows
	void Open_NewLevelWindow();
	void OpenCollisionEditor();

	Console& GetConsole();

	void					RegisterTool(Tool* newTool);

	template<typename T>
	T* GetTool() const
	{
		for (auto& tool : m_Tools)
		{
			if (T* casted = dynamic_cast<T*>(tool))
			{
				return casted;
			}
		}

		return nullptr;
	}

	bool Initialised;
protected:

	/* Create my own dockspace that takes into account the offset of the main menu bar.
	An almost-copy of the logic for creating a default dockstate.*/
	void					InitialiseDockspace(ImVec2 Offset);
	void					RenderApplicationDebug(float DeltaTime);

	bool					OnMouseButtonPressed(MouseButtonPressedEvent& e);
	bool					OnMouseButtonReleased(MouseButtonReleasedEvent& e);
	bool					OnMouseMoved(MouseMovedEvent& e);
	bool					OnMouseScrolled(MouseScrolledEvent& e);
	bool					OnKeyPressed(KeyPressedEvent& e);
	bool					OnKeyTyped(KeyTypedEvent& e);
	bool					OnKeyReleased(KeyReleasedEvent& e);
	bool					OnWindowResized(WindowResizedEvent& e);

	void					UpdateCollisionEditor();

	void					UpdateTools(float DeltaTime);
	void					RenderTools();


	Inspector*						m_Inspector;
	MenuBar*						m_MenuBar;
	AssetWindow* 					m_AssetWindow;
	bool							m_DrawDemoWindow = false;
	Application*					m_ApplicationPointer;
	EditorSettings					m_Settings;
	SceneManager					m_SceneManager;
	ToolBar*						m_Toolbar;
	std::shared_ptr<EditorCamera>	m_EditorCam;
	Console							m_Console;
	LevelManager*					m_LevelManager;
	SpawnWindow*					m_SpawnWindow;

	std::vector<Tool*> m_Tools;


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