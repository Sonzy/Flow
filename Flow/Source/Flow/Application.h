#pragma once
#include "Flowpch.h"
#include "Core.h"
#include "Flow/Window/Window.h"

namespace Flow
{
	class FLOW_API Application
	{
	public:

		Application(const std::string& AppName);
		virtual ~Application();
		
		void Run();

		std::string ApplicationName;

	private:
		std::unique_ptr<Window> MainWindow;
	};

	//Is defined externally
	Application* CreateApplication();
}


