#include <Flow.h>

class ExampleLayer : public Flow::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{

	}

	void OnUpdate() override
	{
		//FLOW_LOG("Update");
	}

	void OnEvent(Flow::Event& e) override
	{
		if(e.GetEventType() == Flow::EventType::WindowClosed)
			FLOW_LOG("{0}", e);
	}
};

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