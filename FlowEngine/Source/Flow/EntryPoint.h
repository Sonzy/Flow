#pragma once

#ifdef FLOW_PLATFORM_WINDOWS

extern Flow::Application* Flow::CreateApplication();

int main(int argc, char** argv)
{
	auto App = Flow::CreateApplication();
	App->Run();
	delete App;
}

#endif