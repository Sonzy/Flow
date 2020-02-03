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
#include <random>

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

		//Get Local File Path
		char Path[128];
		GetModuleFileName(nullptr, Path, sizeof(Path));
		std::string ExeDir = std::string(Path);
		LocalPath = ExeDir.substr(0, ExeDir.find("bin")); 

		std::shared_ptr<StaticMesh> NewMesh = std::make_shared<StaticMesh>("Flow\\Assets\\Models\\Box.obj");
		NewMesh->SetPosition(Vector(1.0f));
		NewMesh->SetScale(Vector(5.0f));
		TestMesh.push_back(NewMesh);

		std::shared_ptr<StaticMesh> NewMesh2 = std::make_shared<StaticMesh>("Flow\\Assets\\Models\\Box.obj");
		NewMesh2->SetPosition(Vector(0.0f, 0.0f, 20.0f));
		TestMesh.push_back(NewMesh2);
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (bRunning)
		{
			float DeltaTime = m_Timer.Mark();

			MainWindow->PreUpdate();

			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate();
			}

			RenderCommand::GetCamera().Tick(DeltaTime);

			Renderer::BeginScene();
			for (auto Mesh : TestMesh)
			{
				Renderer::Submit(reinterpret_cast<Renderable*>(Mesh.get()));
			}
			Renderer::EndScene();


			//ImGui UI Rendering
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
			{
				layer->OnImGuiRender();
			}
			RenderApplicationDebug(DeltaTime);
			RenderCommand::GetCamera().RenderIMGUIWindow();
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

	std::string Application::GetLocalFilePath()
	{
		return LocalPath;
	}

	std::wstring Application::GetLocalFilePathWide()
	{
		return std::wstring(LocalPath.begin(), LocalPath.end());
	}

	Window& Application::GetWindow()
	{
		return *MainWindow;
	}

	void Application::RenderApplicationDebug(float DeltaTime)
	{
		if (ImGui::Begin("Application Statistics"))
		{
			ImGui::Text("Framerate: %.1f", 1 / DeltaTime);
			ImGui::Text("FrameTime: %d ms", (int)(DeltaTime * 1000));
		}
		ImGui::End();
	}

	void Application::SpawnRandomMeshes(std::string LocalMeshPath, int Num)
	{
		std::mt19937 rng(std::random_device{}());
		std::uniform_real_distribution<float> Dist(0.0f, 100.0f);

		for (int i = 0; i < Num; i++)
		{
			std::shared_ptr<StaticMesh> NewMesh = std::make_shared<StaticMesh>(LocalMeshPath);
			NewMesh->SetPosition(Vector(Dist(rng), Dist(rng), Dist(rng)));
			TestMesh.push_back(NewMesh);
		}
	}
}
