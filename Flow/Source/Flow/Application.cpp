#include "Flowpch.h"
#include "Application.h"
#include "Logging/Log.h"
#include "Events/ApplicationEvent.h"

#include "Flow/Input/Input.h"

#include "Flow/Rendering/Core/Camera/CameraLayer.h"

#include "Flow\Rendering\Core\ModelLoader.h"
#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>
#include "ThirdParty\ImGui\imgui.h"

#define BIND_EVENT_FUNCTION(FunctionPtr) std::bind(FunctionPtr, this, std::placeholders::_1)

namespace Flow
{
	Application* Application::Instance = nullptr;

	Application::Application(const std::string& AppName)
		: ApplicationName(AppName)
	{
		Instance = this;

		MainWindow = std::unique_ptr<Window>(Window::Create(WindowProperties(AppName, 1280u, 720u)));
		MainWindow->SetEventCallback(BIND_EVENT_FUNCTION(&Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
		PushLayer(new CameraLayer());

		//Desktop Local
		//D:\\Personal Projects\\Flow\\Flow\
		//Laptop Local
		//C:\Users\Sonny\Documents\Personal Projects\Flow Engine\Flow\

		std::string ModelPath = "C:\\Users\\Sonny\\Documents\\Personal Projects\\Flow Engine\\Flow\\Assets\\Models\\Box.obj";

		int Count = 10;
		float Distance = 5.0f;
		int HalfCount = Count / 2;
		for (int i = 0; i < Count; i++)
		{
			for (int j = 0; j < Count; j++)
			{
				for (int k = 0; k < Count; k++)
				{
					std::shared_ptr<StaticMesh> NewMesh = std::make_shared<StaticMesh>(ModelPath);
					NewMesh->SetPosition(Vector((-HalfCount + i) * Distance, (-HalfCount + j) * Distance, (-HalfCount + k) * Distance));
					TestMesh.push_back(NewMesh);

					FLOW_ENGINE_LOG("{0}, {1}, {2}", i, j, k);
				}
			}
		}
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (bRunning)
		{
			MainWindow->PreUpdate();

			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate();
			}

			Renderer::BeginScene();
			for (auto Mesh : TestMesh)
			{
				Renderer::Submit(reinterpret_cast<Renderable*>(Mesh.get()));
			}
			Renderer::EndScene();

			//Allow each layer to render their own IMGUI
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
			{
				layer->OnImGuiRender();
			}

			m_ImGuiLayer->End();

			MainWindow->OnUpdate();
			MainWindow->PostUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher Dispatcher(e);
		Dispatcher.Dispatch<WindowClosedEvent>(BIND_EVENT_FUNCTION(&Application::OnWindowClosed));

		for (auto iterator = m_LayerStack.end(); iterator != m_LayerStack.begin();)
		{
			(*--iterator)->OnEvent(e);
			if (e.bHandled)
				break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	bool Application::OnWindowClosed(WindowClosedEvent& e)
	{
		bRunning = false;
		FLOW_ENGINE_LOG("Window Closed");
		return true;
	}

	Application& Application::GetApplication()
	{
		return *Instance;
	}

	Window& Application::GetWindow()
	{
		return *MainWindow;
	}
}
