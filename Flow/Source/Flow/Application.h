#pragma once
#include "Flowpch.h"
#include "Core.h"
#include "Flow/Window/Window.h"
#include "Layers/LayerStack.h"

#include "Events/ApplicationEvent.h"

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

		Window& GetWindow();

	public:
		std::string ApplicationName;

	private:
		static Application* Instance;
		std::unique_ptr<Window> MainWindow;
		bool bRunning = true;

		LayerStack m_LayerStack;
	};

	//Is defined externally
	Application* CreateApplication();
}


