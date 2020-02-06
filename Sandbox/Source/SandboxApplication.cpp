#include <Flow.h>
#include "Example Layer.h"

class Sandbox : public Flow::Application
{
public:
	Sandbox(const std::string& AppName)
		: Application(AppName)
	{
		ExampleLayer* Example = new ExampleLayer();
		PushLayer(Example);
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