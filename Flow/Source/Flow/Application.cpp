#include "Flowpch.h"
#include "Application.h"
#include "Logging/Log.h"
#include "Events/ApplicationEvent.h"

namespace Flow
{
	Application::Application(const std::string& AppName)
		: ApplicationName(AppName)
	{
		MainWindow = std::unique_ptr<Window>(Window::Create(WindowProperties()));
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (true)
		{
			MainWindow->OnUpdate();
		}
	}
}
