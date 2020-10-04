#pragma once
#include "Flow/Layers/Layer.h"
#include "Flow/Events/MouseEvent.h"
#include "Flow/Editor/SceneManager.h"

//= Forward Declarations ================================

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

//= Class Declaration =========================================

class Editor : public Layer
{
public:

	//= Public Structs =========================================
	struct Settings
	{
		Settings()
			: m_ObjectHighlightColour(0.0f, 1.0f, 0.0f)
		{}

		Vector3 m_ObjectHighlightColour;
	};

public:

	//= Public Functions =========================================

								Editor();
								~Editor();

	//= Layer interface =

	void						Initialise();
	virtual void				BeginPlay() override;
	virtual void				OnAttach() override;
	virtual void				OnDetach() override;
	virtual void				OnImGuiRender(bool DrawEditor) override;
	void						OnEvent(Event& e) override;
	virtual void				OnUpdate(float DeltaTime) override;

	//=

	static Editor*				GetEditor();
	static Editor::Settings&	GetEditorSettings();
	Inspector*					GetInspector() const;
	MenuBar*					GetMenuBar() const;
	bool						IsInitialised() const { return m_Initialised; }

	void						SetDemoWindowVisible(bool Enabled);
	void						ToggleImGuiDemoWindow();

	bool						IsSceneWindowFocused() const;
	bool						IsMouseOverScene() const;

	IntVector2					GetSceneWindowSize() const;
	IntVector2					GetSceneWindowPosition() const;

	//Open Windows
	void						Open_NewLevelWindow();

	Console&					GetConsole();

	void						RegisterTool(Tool* newTool);

	//= Public Template Functions =========================================

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


protected:

	//= Protected Functions ============================================

	/* Create my own dockspace that takes into account the offset of the main menu bar.
	An almost-copy of the logic for creating a default dockstate.*/
	void							InitialiseDockspace(ImVec2 Offset);
	void							RenderApplicationDebug(float DeltaTime);

	bool							OnMouseButtonPressed(MouseButtonPressedEvent& e);
	bool							OnMouseButtonReleased(MouseButtonReleasedEvent& e);
	bool							OnMouseMoved(MouseMovedEvent& e);
	bool							OnMouseScrolled(MouseScrolledEvent& e);
	bool							OnKeyPressed(KeyPressedEvent& e);
	bool							OnKeyTyped(KeyTypedEvent& e);
	bool							OnKeyReleased(KeyReleasedEvent& e);
	bool							OnWindowResized(WindowResizedEvent& e);

	void							UpdateCollisionEditor();

	void							UpdateTools(float DeltaTime);
	void							RenderTools();

	//= Protected Variables ============================================


	bool							m_Initialised;
	Inspector*						m_Inspector;
	MenuBar*						m_MenuBar;
	AssetWindow* 					m_AssetWindow;
	bool							m_DrawDemoWindow = false;
	Application*					m_ApplicationPointer;
	Editor::Settings				m_Settings;
	SceneManager					m_SceneManager;
	ToolBar*						m_Toolbar;
	std::shared_ptr<EditorCamera>	m_EditorCam;
	Console							m_Console;
	LevelManager*					m_LevelManager;
	SpawnWindow*					m_SpawnWindow;

	std::vector<Tool*>				m_Tools;


	//App Statistics
	float							m_TimeSinceFrameUpdate = 0.0f;
	int								m_FrameCounter = 0;
	float							m_FrameTimer = 0.0f;
	float							m_FrameDeltaTime = 0.0f;
	float							m_LastFrameTime = 0.0f;
	const float						m_UpdateInterval = 1.0f;

	//Editor Window
	Vector2							m_EditorViewportSize;
};