#pragma once
#include "Logging/Log.h"
#include "Application.h"

#ifdef FLOW_PLATFORM_WINDOWS

extern Application* CreateApplication();

int main(int argc, char** argv)
{
	Log::InitialiseEngineLogger();
	auto App = CreateApplication();
	Log::InitialiseApplicationLogger(App->ApplicationName);


	App->Run();
	delete App;

	return 0;
}

#endif