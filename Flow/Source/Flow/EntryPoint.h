#pragma once
#include "Logging/Log.h"
#include "Application.h"

#ifdef FLOW_PLATFORM_WINDOWS

extern Flow::Application* Flow::CreateApplication();

int main(int argc, char** argv)
{
	Flow::Log::InitialiseEngineLogger();
	auto App = Flow::CreateApplication();
	Flow::Log::InitialiseApplicationLogger(App->ApplicationName);

	App->InitialiseApplication();


	App->Run();
	delete App;

	return 0;
}

#endif