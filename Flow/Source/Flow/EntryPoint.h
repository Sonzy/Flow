#pragma once
#include "Logging/Log.h"
#include "Application.h"

#ifdef FLOW_PLATFORM_WINDOWS

extern Flow::Application* Flow::CreateApplication();

int main(int argc, char** argv)
{
	Flow::Log::Initialise();

	auto App = Flow::CreateApplication();
	App->Run();
	delete App;

	return 0;
}

#endif