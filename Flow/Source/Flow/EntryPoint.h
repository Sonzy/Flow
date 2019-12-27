#pragma once
#include "Logging/Log.h"
#include "Application.h"

#ifdef FLOW_PLATFORM_WINDOWS

extern Flow::Application* Flow::CreateApplication();

int main(int argc, char** argv)
{
	auto App = Flow::CreateApplication();
	Flow::Log::Initialise(App->ApplicationName);

	App->Run();
	delete App;

	return 0;
}

#endif