#include <Flow.h>

class Sandbox : public Flow::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};


Flow::Application* Flow::CreateApplication()
{
	Sandbox* App = new Sandbox();
	return App;
}