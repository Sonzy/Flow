#pragma once
#include "Flowpch.h"
#include "Core.h"
#include "Flow/Window/Window.h"
#include "Layers/LayerStack.h"
#include "Flow\Helper\Timer.h"

#include "Events/ApplicationEvent.h"

#include "UserInterface/imgui/ImGuiLayer.h"

namespace Flow
{
	class World;
	class Inspector;
	class EditorLayer;
	class SelectionGizmo;

	class FLOW_API Application
	{
	

	public:

		Application(const std::string& AppName);
		virtual ~Application();
		
		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		bool OnWindowClosed(WindowClosedEvent& e);
		bool OnWindowResized(WindowResizedEvent& e);

		static Application& GetApplication();
		static World* GetWorld();

		/* Returns path to Flow solution directory */
		std::string GetLocalFilePath();
		std::wstring GetLocalFilePathWide();

		Inspector* GetInspector();

		Window& GetWindow();
	public:
		std::string ApplicationName;

	private:

		void RenderApplicationDebug(float DeltaTime);

	private:

		//= Application =============

		static Application* Instance;
		std::unique_ptr<Window> MainWindow_;

		ImGuiLayer* ImGuiLayer_;
		EditorLayer* EditorLayer_;

		bool Running_ = true;
		bool Paused_ = false;
		bool DrawCollision_ = false;

		//= Game ====================

		World* GameWorld_;

		//= Editor ==================

		Inspector* Inspector_;
		SelectionGizmo* SelectionGizmo_;

		//= Debug ===================

		float TimeSinceFrameRateCheck = 0.0f;
		int FrameCounter = 0;
		float FrameTimer = 0.0f;

		//= Helper ===============

		LayerStack LayerStack_;
		Timer Timer_;

		//= Paths =================

		std::string LocalPath_;
	};

	//Is defined externally
	Application* CreateApplication();
}


