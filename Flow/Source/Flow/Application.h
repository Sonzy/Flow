#pragma once
#include "Core.h"

//= Standard Includes =
#include "Flow/Window/Window.h"
#include "Layers/LayerStack.h"

//= Event Includes =
#include "Events/ApplicationEvent.h"

//= Helper Includes =
#include "UserInterface/imgui/ImGuiLayer.h"
#include "Utils/Timer.h"
#include "Utils/FileSystem.h"

//= Forward Declarations ===============================

class Application;
class World;
class Inspector;
class Editor;
class ClassFactory;
class GameLayer;
class Layer;

//= External Functions =================================

Application* CreateApplication();

class FLOW_API Application
{
public:

	//= Public Static Functions ================================

	static Application&					Get();
	static World*						GetWorld();
	static void							Shutdown();

	static void							SaveLevel();
	static void							LoadLevel();
	static void							NewLevel();
	static void							SavePlayState();
	static void							LoadPlayState();

	static fs::path						GetEnginePath() { return Get().m_EngineDirectory; };
	static fs::path						GetGamePath()	{ return Get().m_GameDirectory; };

	//= Public Functions =======================================

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
	Editor*								GetEditor()				{ return m_Layer_Editor; };
#endif

	static bool							StartGame();
	static bool							PauseGame();
	static bool							StopGame();

	ClassFactory&						GetClassFactory()		{ return *m_ClassFactory; }

public:

	//= Public Variables =======================================

	std::string							m_ApplicationName;

	//= Public Editor Variables  ===============================
#if WITH_EDITOR
	bool								m_RenderEditor = true;
#endif


private:
	//Allow editor to control local variables
	friend class Editor;

	//= Private Static Variables ================================

	static Application*					sm_Application;

	//= Private Variables ================================

	Window*								m_MainWindow;

	//= Layers =

	ImGuiLayer*							m_Layer_ImGui;
	GameLayer*							m_Layer_Game;
#if WITH_EDITOR
	Editor*								m_Layer_Editor;
#endif

	bool								m_Running;
	bool								m_GamePaused;
	bool								m_DrawCollision;

	//= Game ====================

	World*								m_GameWorld;

	//= Debug ===================



	//= Helper ===============

	LayerStack							m_LayerStack;
	Timer								m_Timer;
	ClassFactory*						m_ClassFactory;

	//= Paths =================

	fs::path							m_EngineDirectory;
	fs::path							m_GameDirectory;
};

