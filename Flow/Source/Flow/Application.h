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

		static Application& GetApplication();

		/* Returns path to Flow solution directory */
		std::string GetLocalFilePath();
		std::wstring GetLocalFilePathWide();

		Window& GetWindow();

	public:
		std::string ApplicationName;

	private:

		void RenderApplicationDebug(float DeltaTime);

		void SpawnRandomMeshes(std::string LocalMeshPath, int Num);

	private:
		static Application* Instance;
		std::unique_ptr<Window> MainWindow;
		ImGuiLayer* m_ImGuiLayer;
		bool bRunning = true;

		LayerStack m_LayerStack;
		Timer m_Timer;

		std::vector<std::shared_ptr<Flow::StaticMesh>> TestMesh;
		std::string LocalPath;
	};

	//Is defined externally
	Application* CreateApplication();
}


