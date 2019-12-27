#pragma once
#include "Core.h"
#include <string>

namespace Flow
{
	class FLOW_API Application
	{
	public:

		Application(const std::string& AppName);
		virtual ~Application();
		
		void Run();

		std::string ApplicationName;
	};

	//Is defined externally
	Application* CreateApplication();
}


