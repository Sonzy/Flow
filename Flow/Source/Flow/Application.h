#pragma once
#include "Flowpch.h"
#include "Core.h"
#include "Flow/Window/Window.h"
#include "Layers/LayerStack.h"
#include "Flow\Helper\Timer.h"

#include "Events/ApplicationEvent.h"

#include "UserInterface/imgui/ImGuiLayer.h"

#include "Flow/Rendering/Core/Mesh/StaticMesh.h"

namespace Flow
{
	class World;
	class Inspector;
	class EditorLayer;

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
		std::unique_ptr<Window> MainWindow;

		ImGuiLayer* m_ImGuiLayer;
		EditorLayer* m_EditorLayer;

		bool bRunning = true;

		//= Game ====================

		World* GameWorld;

		//= Editor ==================

		Inspector* m_Inspector;

		//= Debug ===================

		float TimeSinceFrameRateCheck = 0.0f;
		int FrameCounter = 0;
		float FrameTimer = 0.0f;

		//= Helper ===============

		LayerStack m_LayerStack;
		Timer m_Timer;

		//= Paths =================

		std::string LocalPath;
	};

	//Is defined externally
	Application* CreateApplication();
}


