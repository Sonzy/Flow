#include "Flowpch.h"
#include <Psapi.h> //memory debug
#include "Application.h"
#include "Logging/Log.h"
#include "Events/ApplicationEvent.h"

#include "Flow/Input/Input.h"
#include "Flow/Rendering/Core/Camera/CameraLayer.h"
#include "ThirdParty\ImGui\imgui.h"

#include "Flow\Assets\AssetSystem.h"

#include "Flow\GameFramework\World.h"

#include "Flow\Editor\Inspector.h"

//TODO: Load somewhere else
#include "Flow\Assets\Materials\Mat_FlatColour.h"
#include "Flow\Assets\Materials\Temporary\Mat_Hat_FancyMan.h"

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

		//TODO: Load assets somewhere
		//= Models =
		AssetSystem::LoadAsset("Box2", "Flow/Assets/Models/Box2.obj");
		AssetSystem::LoadAsset("Box", "Flow/Assets/Models/Box.obj");
		AssetSystem::LoadAsset("WeirdBox", "Flow/Assets/Models/WeirdBox.obj"); 
		AssetSystem::LoadAsset("Hat_FancyMan", "Flow/Assets/Models/Hat_FancyMan.obj");
		AssetSystem::LoadAsset("Plane", "Flow/Assets/Models/Plane.obj");

		//= Textures =
		AssetSystem::LoadAsset("ExampleRed", "Flow/Assets/Textures/ExampleRed.png");
		AssetSystem::LoadAsset("TestTexture", "Flow/Assets/Textures/TestTexture.png");
		AssetSystem::LoadAsset("TestTextureFlip", "Flow/Assets/Textures/TestTextureFlip.png");
		AssetSystem::LoadAsset("CharacterTexture", "Flow/Assets/Textures/CharacterTexture.png"); 

		//= Shaders =
		AssetSystem::LoadAsset("TexturedLightVS", "Flow/Source/Flow/Rendering/Core/Shaders/TexturedPerPixelVS.cso");
		AssetSystem::LoadAsset("TexturedLightPS", "Flow/Source/Flow/Rendering/Core/Shaders/TexturedPerPixelPS.cso");
		AssetSystem::LoadAsset("TexturedVS", "Flow/Source/Flow/Rendering/Core/Shaders/TextureVS.cso");
		AssetSystem::LoadAsset("TexturedPS", "Flow/Source/Flow/Rendering/Core/Shaders/TexturePS.cso");
		AssetSystem::LoadAsset("SolidColourVS", "Flow/Source/Flow/Rendering/Core/Shaders/SolidColorVS.cso");
		AssetSystem::LoadAsset("SolidColourPS", "Flow/Source/Flow/Rendering/Core/Shaders/SolidColourPS.cso");

		//= Materials =
		AssetSystem::CreateMaterial<Mat_FlatColour>("Mat_FlatColour");
		AssetSystem::CreateMaterial<Mat_Hat_FancyMan>("Mat_HatFancyMan");

		//Create the game world
		GameWorld = new World("Game World");

		m_Inspector = new Inspector();
	}

	Application::~Application()
	{
		FLOW_ENGINE_LOG("Shutting Down Engine");

		RenderCommand::Shutdown();
		AssetSystem::Shutdown();
	}

	void Application::Run()
	{
		while (bRunning)
		{
			float DeltaTime = m_Timer.Mark();

			MainWindow->PreUpdate();
			MainWindow->OnUpdate();

			//TODO: Check where to move the world since I'm using layers
			GameWorld->Tick(DeltaTime);
			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate(DeltaTime);
			}

			//= UI Rendering =

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
			{
				layer->OnImGuiRender();
			}

			m_Inspector->SetCurrentWorld(GameWorld);
			m_Inspector->RenderInspector();
			m_Inspector->RenderHeirarchy();
			RenderApplicationDebug(DeltaTime);

			m_ImGuiLayer->End();

			//= Post Update =

			MainWindow->PostUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher Dispatcher(e);
		Dispatcher.Dispatch<WindowClosedEvent>(BIND_EVENT_FUNCTION(&Application::OnWindowClosed));
		Dispatcher.Dispatch<WindowResizedEvent>(BIND_EVENT_FUNCTION(&Application::OnWindowResized));

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

	bool Application::OnWindowResized(WindowResizedEvent& e)
	{
		FLOW_ENGINE_LOG("Window Resized to ({0}, {1})", e.GetWidth(), e.GetHeight());
		//MainWindow->Resize(e.GetWidth(), e.GetHeight());
		RenderCommand::Resize(e.GetWidth(), e.GetHeight());
		return false;
	}

	Application& Application::GetApplication()
	{
		return *Instance;
	}

	World* Application::GetWorld()
	{
		return Instance->GameWorld;
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
		TimeSinceFrameRateCheck += DeltaTime;
		FrameCounter++;

		if (FrameCounter > 5)
		{
			FrameTimer = TimeSinceFrameRateCheck / 5;
			TimeSinceFrameRateCheck = 0;
			FrameCounter = 0;
		}	

		if (ImGui::Begin("Application Statistics"))
		{
			ImGui::Text("Framerate: %.1f", 1 / FrameTimer);
			ImGui::Text("FrameTime: %.1f ms", DeltaTime * 1000);

			PROCESS_MEMORY_COUNTERS MemoryData;
			GetProcessMemoryInfo(GetCurrentProcess(), &MemoryData, sizeof(MemoryData));

			ImGui::Text("Memory: %.1f MB", (float)MemoryData.WorkingSetSize / 1048576);
		}
		ImGui::End();
	}
}
