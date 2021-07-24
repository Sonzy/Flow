#pragma once

// Includes ///////////////////////////////////////////////////////////////////////

#include "Core.h"
#include "Flow/Window/Window.h"
#include "Framework/Layers/LayerStack.h"
#include "Framework/Events/ApplicationEvent.h"
#include "UserInterface/imgui/ImGuiLayer.h"
#include "Framework/Utils/Timer.h"
#include "Framework/Utils/FileSystem.h"

// Forward Declarations ///////////////////////////////////////////////////////////

class Application;
class World;
class Inspector;
class Editor;
class ClassFactory;
class GameLayer;
class Layer;

// Global Functions ///////////////////////////////////////////////////////////////

Application* CreateApplication();

// Class Definition ///////////////////////////////////////////////////////////////

class FLOW_API Application
{
	friend class Editor;

public:

	// Public Static Functions ////////////////////////////////////////////////////

	static Application&					Get();
	static World*						GetWorld();
	static void							Shutdown();

	static void							SaveLevel();
	static void							LoadLevel();
	static void							NewLevel();

	static fs::path						GetEnginePath();
	static fs::path						GetGamePath();

public:

	// Public Functions ///////////////////////////////////////////////////////////

										Application(const std::string& AppName);
	virtual void						InitialiseApplication();
	virtual								~Application();

	void								Run();

	void								OnEvent(Event& e);

	void								PushLayer(Layer* layer);
	void								PushOverlay(Layer* layer);

	bool								OnWindowClosed(WindowClosedEvent& e);
	bool								OnWindowResized(WindowResizedEvent& e);
	bool								OnWindowMinimized(WindowMinimizedEvent& e);
	bool								OnWindowRestored(WindowRestoredEvent& e);

	Window&								GetWindow();

#if WITH_EDITOR
	Editor*								GetEditor();
	bool								IsRenderingEditor();
	void								SetEditorRenderingEnabled(bool enabled);
#endif

	static bool							StartGame();

	ClassFactory&						GetClassFactory();

	const std::string&					GetName() const;

private:

	// Private Static Variables ///////////////////////////////////////////////////

	static Application*					sm_Application;

	// Private Variables //////////////////////////////////////////////////////////

	Window*								m_window;

	// Layers //

	GameLayer*							m_gameLayer;

	// Editor //

#if WITH_EDITOR

	ImGuiLayer*							m_imGuiLayer;
	Editor*								m_editorLayer;
	bool								m_renderEditor;
	bool								m_drawCollision;

#endif

	bool								m_running;


	// Game //

	World*								m_world;

	// Helper //

	LayerStack							m_layerStack;
	Timer								m_timer;
	ClassFactory*						m_classFactory;

	// Paths //

	fs::path							m_engineDirectory;
	fs::path							m_gameDirectory;

	std::string							m_name;


};

// Inline Function Definitions ////////////////////////////////////////////////////

inline ClassFactory& Application::GetClassFactory()
{ 
	return *m_classFactory; 
}

inline fs::path Application::GetEnginePath() 
{ 
	return Get().m_engineDirectory; 
};

inline fs::path Application::GetGamePath() 
{ 
	return Get().m_gameDirectory; 
};

inline const std::string& Application::GetName() const
{
	return m_name;
}

#if WITH_EDITOR

inline Editor* Application::GetEditor()
{ 
	return m_editorLayer; 
};

inline bool	Application::IsRenderingEditor()
{
	return m_renderEditor;
}

inline void	Application::SetEditorRenderingEnabled(bool enabled)
{
	m_renderEditor = enabled;
}

#endif