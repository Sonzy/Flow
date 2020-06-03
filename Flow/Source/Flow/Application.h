#pragma once
#include "Flowpch.h"
#include "Core.h"
#include "Flow/Window/Window.h"
#include "Layers/LayerStack.h"
#include "Flow\Helper\Timer.h"

#include "Events/ApplicationEvent.h"

#include "UserInterface/imgui/ImGuiLayer.h"

class World;
class Inspector;
class EditorLayer;

class FLOW_API Application
{
public:

	Application(const std::string& AppName);
	virtual void InitialiseApplication();
	virtual ~Application();

	void Run();

	void OnEvent(Event& e);

	void PushLayer(Layer* layer);
	void PushOverlay(Layer* layer);

	bool OnWindowClosed(WindowClosedEvent& e);
	bool OnWindowResized(WindowResizedEvent& e);

	static Application& GetApplication();
	static World* GetWorld();
	static void Shutdown();

	static void SaveLevel();
	static void LoadLevel();

	/* Returns path to Flow solution directory */
	std::string GetLocalFilePath();
	std::wstring GetLocalFilePathWide();

	static Window* CreateNewWindow(const std::string& WindowName);
	static bool RegisterWindow(Window* NewWindow);
	static bool DeRegisterWindow(Window* Window);

	Window& GetWindow();

#if WITH_EDITOR
	EditorLayer* GetEditor() { return EditorLayer_; };
	bool _DrawEditor = true;
#else
	EditorLayer* GetEditor() { return nullptr; };
#endif

	static void StartGame();
	static void PauseGame();
	static void StopGame();

public:
	std::string ApplicationName;

private:


	void UpdateWindowDestruction();


private:
	friend class EditorLayer;
	//= Application =============

	static Application* s_Instance;

	std::unique_ptr<Window> _MainWindow;

	std::vector<Window*> _Windows;
	std::vector<Window*> _WindowsToDestroy;

	ImGuiLayer* ImGuiLayer_;

#if WITH_EDITOR
	EditorLayer* EditorLayer_;
#endif

	bool _Running = true;
	bool Paused_ = false;
	bool DrawCollision_ = false;
	bool _UpdatingChildWindows = false;


	//= Game ====================

	World* GameWorld_;

	//= Debug ===================



	//= Helper ===============

	LayerStack LayerStack_;
	Timer Timer_;

	//= Paths =================

	std::string LocalPath_;
};

//Is defined externally
Application* CreateApplication();