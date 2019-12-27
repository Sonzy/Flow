#include <Flow.h>

class Sandbox : public Flow::Application
{
public:
	Sandbox(const std::string& AppName)
		: Application(AppName)
	{

	}

	~Sandbox()
	{

	}
};


Flow::Application* Flow::CreateApplication()
{
	Sandbox* App = new Sandbox("Sandbox");
	return App;
}