#pragma once
#include "Core.h"

namespace Flow
{
	class FLOW_API Application
	{
	public:

		Application();
		virtual ~Application();
		
		void Run();
	};

	//Is defined externally
	Application* CreateApplication();
}


