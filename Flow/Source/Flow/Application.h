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
class SelectionGizmo;

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

	/* Returns path to Flow solution directory */
	std::string GetLocalFilePath();
	std::wstring GetLocalFilePathWide();

	Window& GetWindow();

	EditorLayer* GetEditor() { return EditorLayer_; };
	bool _DrawEditor = true;
public:
	std::string ApplicationName;


private:
	friend class EditorLayer;
	//= Application =============

	static Application* Instance;
	std::unique_ptr<Window> MainWindow_;

	ImGuiLayer* ImGuiLayer_;
	EditorLayer* EditorLayer_;

	bool _Running = true;
	bool Paused_ = false;
	bool DrawCollision_ = false;


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