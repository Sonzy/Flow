#pragma once
#include "Layers/Layer.h"
#include "Events/MouseEvent.h"

#include "Editor/UIComponents/SceneManager.h"
#include "Editor/Other/IconBatcher.h"

//= Forward Declarations ================================

class UIComponent;
class MenuBar;
class Application;
struct ImVec2;
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

// Class Declaration ///////////////////////////////////////////////

class Editor : public Layer
{
	friend class ImGuiLayer;
public:

	// Public Structs ///////////////////////////////////////////////
	struct Settings
	{
		Settings()
			: m_ObjectHighlightColour(0.0f, 1.0f, 0.0f)
		{}

		Vector3				m_ObjectHighlightColour;
		std::string			m_StartingLevel;
	};

	class SettingsWindow
	{
	public:

		// Public Functions ///////////////////////////////////////////////

		void				Render(Editor::Settings& EditorSettings, Editor& EditorRef);

	private:

		// Private Variables ///////////////////////////////////////////////

		char				m_StartingNameBuffer[128] = { '\0' };
	};

public:

// Public Static Functions ///////////////////////////////////////////

	static Editor& Get();
	static Editor::Settings& GetEditorSettings();

public:

// Public Functions ///////////////////////////////////////////////

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

	//= Scene Window =

	bool						IsSceneWindowFocused() const;
	bool						IsMouseOverScene() const;
	IntVector2					GetSceneWindowSize() const;
	IntVector2					GetSceneWindowPosition() const;

	//= Window Visibilty =

	void						Open_NewLevelWindow();

	//= ImGui Testing =

	void						SetDemoWindowVisible(bool Enabled);
	void						ToggleImGuiDemoWindow();

	//= Miscellanious =

	bool						IsInitialised() const;
	void						ShowSettingsWindow(bool Show);

	//= Custom Tools =

	IconBatcher&				GetIconBatcher();

public:

	// Public Template Functions ///////////////////////////////////////////////

	template<typename T>
	T* GetTool() const
	{
		for (Tool* tool : m_Tools)
		{
			if (T* casted = dynamic_cast<T*>(tool))
			{
				return casted;
			}
		}

		return nullptr;
	}

	template<typename T>
	T* GetUIComponent() const
	{
		for (UIComponent* component : m_UIComponents)
		{
			if (T* casted = dynamic_cast<T*>(component))
			{
				return casted;
			}
		}

		return nullptr;
	}

	template<typename T>
	void RegisterTool()
	{
		static_assert(std::is_base_of<Tool, T>::value, "Tried to create a component templated with a non-component type");

		if (GetTool<T>() != nullptr)
		{
			FLOW_ENGINE_WARNING("Editor::RegisterTool: Already registered tool");
			return;
		}

		m_Tools.push_back(new T());
	}

	template<typename T>
	void						RegisterUIComponent()
	{
		static_assert(std::is_base_of<UIComponent, T>::value, "Tried to create a component templated with a non-component type");

		if (GetUIComponent<T>() != nullptr)
		{
			FLOW_ENGINE_WARNING("Editor::RegisterUIComponent: Already registered UI Component");
			return;
		}

		T* NewUIComponent = new T();
		NewUIComponent->m_Editor = this;
		m_UIComponents.push_back(NewUIComponent);
	}

private:

	// Protected Functions ///////////////////////////////////////////////

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

	//= Saving/Loading =

	void							SaveEditorSettings();
	void							LoadEditorSettings();

	// Protected Variables ///////////////////////////////////////////////


	bool							m_Initialised;
	bool							m_ShowSettingsWindow;
	MenuBar*						m_MenuBar;
	bool							m_DrawDemoWindow = false;
	Application*					m_ApplicationPointer;
	Editor::Settings				m_Settings;
	EditorCamera*					m_EditorCam;
	LevelManager*					m_LevelManager;
	IconBatcher						m_IconBatcher;

	//= Cached UI Components =

	SceneManager*					m_SceneManager;

	Editor::SettingsWindow			m_SettingsWindow;

	std::vector<Tool*>				m_Tools;
	std::vector<UIComponent*>		m_UIComponents;

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