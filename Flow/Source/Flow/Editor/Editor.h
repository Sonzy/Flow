#pragma once

#if WITH_EDITOR

// Includes /////////////////////////////////////////////////////////

#include "Editor/UIComponents/SceneManager.h"
#include "Framework/Layers/Layer.h"
#include "Framework/Events/MouseEvent.h"
#include "Maths/Vector4.h"

// Forward Declarations /////////////////////////////////////////////////////////

class Application;
class Console;
class EditorCamera;
class IconManager;
class KeyPressedEvent;
class KeyReleasedEvent;
class KeyTypedEvent;
class LevelManager;
class MenuBar;
class Module;
class MouseButtonPressedEvent;
class MouseButtonReleasedEvent;
class MouseMovedEvent;
class WindowResizedEvent;
class MouseScrolledEvent;
class Tool;
class UIComponent;

struct ImVec2;

// Class Declaration ///////////////////////////////////////////////

class Editor : public Layer
{
	friend class ImGuiLayer;
public:

	// Public Structs ///////////////////////////////////////////////
	struct Settings
	{
		Settings();

		// Levels //

		std::string			m_StartingLevel;

		// Docking //

		bool				m_DockFloatingWindows;
		Vector2				m_DockPadding;

		// Console //

		Vector4				m_consoleLogColor;
		Vector4				m_consoleWarningColor;
		Vector4				m_consoleErrorColor;

		// Editor Camera //

		float				m_cameraSpeed;
		float				m_cameraPanningSpeed;
		float				m_cameraScrollingSpeed;

		// Other //

		Vector3				m_ObjectHighlightColour;

		// Rendering //

		float				m_nearPlane;
		float				m_farPlane;

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

	// Public Static Functions ///////////////////////////////////////////////

	static Editor&				Get();
	static Editor::Settings&	GetSettings();

	// Public Functions ///////////////////////////////////////////////

								Editor();
								~Editor();

	//= Layer interface =

	void						Initialise();
	void						InitialiseEditor(); //Called after world initialised
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

	//= Window visibility=

	void						Open_NewLevelWindow();

	//= ImGui Testing =

	void						SetDemoWindowVisible(bool Enabled);
	void						ToggleImGuiDemoWindow();
	void						ImGuiPrototypingWindow();

	//= Miscellanious =

	bool						IsInitialised() const;
	void						ShowSettingsWindow(bool Show);

	// Public Template Functions ///////////////////////////////////////////////

	template<typename T>
	T* GetTool() const;

	template<typename T>
	T* GetUIComponent() const;

	template<typename T>
	T* GetModule() const;

	template<typename T>
	void RegisterTool();

	template<typename T>
	void RegisterUIComponent();

	template<typename T>
	void RegisterModule();

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

	//= ImGui Window Options =

	bool							m_ShowSettingsWindow;
	bool							m_ShowPrototypingWindow;
	bool							m_DrawDemoWindow;

	//= References =

	MenuBar*						m_MenuBar;
	Application*					m_ApplicationPointer;
	EditorCamera*					m_EditorCam;
	LevelManager*					m_LevelManager;
	Editor::Settings				m_Settings;

	//= Cached UI Components =

	SceneManager*					m_SceneManager;
	Editor::SettingsWindow			m_SettingsWindow;

	std::vector<Tool*>				m_Tools;
	std::vector<UIComponent*>		m_UIComponents;
	std::vector<Module*>			m_modules;

	//App Statistics
	float							m_TimeSinceFrameUpdate = 0.0f;
	int								m_FrameCounter = 0;
	float							m_FrameTimer = 0.0f;
	float							m_FrameDeltaTime = 0.0f;
	float							m_LastFrameTime = 0.0f;
	const float						m_UpdateInterval = 1.0f;

	//Editor Window
	Vector2							m_EditorViewportSize;

	// Debug Options //

	bool							m_drawCollision;
};

// Inline Function Definitions ////////////////////////////////////

inline Editor::Settings::Settings()
	: m_StartingLevel("")
	, m_DockFloatingWindows(true)
	, m_DockPadding(10.0f, 10.0f)
	, m_consoleLogColor(1.0f, 1.0f, 1.0f, 1.0f)
	, m_consoleWarningColor(1.0f, 1.0f, 0.0f, 1.0f)
	, m_consoleErrorColor(1.0f, 0.0f, 0.0f, 1.0f)
	, m_cameraSpeed(10.0f)
	, m_cameraPanningSpeed(0.5f)
	, m_cameraScrollingSpeed(0.5f)
	, m_ObjectHighlightColour(0.0f, 1.0f, 0.0f)
	, m_nearPlane(0.5f)
	, m_farPlane(2000.0f)
{

}

inline bool	Editor::IsInitialised() const 
{ 
	return m_Initialised; 
}

inline void	Editor::ShowSettingsWindow(bool Show) 
{ 
	m_ShowSettingsWindow = Show;
}

template<typename T>
inline T* Editor::GetTool() const
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
inline T* Editor::GetUIComponent() const
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
inline T* Editor::GetModule() const
{
	for (Module* module : m_modules)
	{
		if (T* casted = dynamic_cast<T*>(module))
		{
			return casted;
		}
	}

	return nullptr;
}

template<typename T>
inline void Editor::RegisterTool()
{
	static_assert(std::is_base_of<Tool, T>::value, "Tried to create a component Tool with a non-Tool type");

	if (GetTool<T>() != nullptr)
	{
		FLOW_ENGINE_WARNING("Editor::RegisterTool: Already registered tool");
		return;
	}

	m_Tools.push_back(new T());
}

template<typename T>
inline void Editor::RegisterUIComponent()
{
	static_assert(std::is_base_of<UIComponent, T>::value, "Tried to create a UIComponent templated with a non-UIComponent type");

	if (GetUIComponent<T>() != nullptr)
	{
		FLOW_ENGINE_WARNING("Editor::RegisterUIComponent: Already registered UI Component");
		return;
	}

	T* NewUIComponent = new T();
	NewUIComponent->m_editor = this;
	m_UIComponents.push_back(NewUIComponent);
}

template<typename T>
inline void Editor::RegisterModule()
{
	static_assert(std::is_base_of<Module, T>::value, "Tried to create a module templated with a non-module type");

	if (GetModule<T>() != nullptr)
	{
		FLOW_ENGINE_WARNING("Editor::RegisterUIComponent: Already registered Module");
		return;
	}

	T* newModule = new T();
	newModule->m_editor = this;
	m_modules.push_back(newModule);
}

#endif //WITH_EDITOR