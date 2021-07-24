#pragma once
#include "Framework/Logging/Log.h"
#include "Application.h"

#ifdef FLOW_PLATFORM_WINDOWS

extern Application* CreateApplication();

int main(int argc, char** argv)
{
	Log::InitialiseEngineLogger();

	Application* App = CreateApplication();
	Log::InitialiseApplicationLogger(App->GetName());

	App->InitialiseApplication();

	App->Run();

	delete App;

	return 0;
}

#endif