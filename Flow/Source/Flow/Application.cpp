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
#include "Flow\Layers\EditorLayer.h"
#include "Flow\Editor\SelectionGizmo.h"

//TODO: Load somewhere else
#include "Flow\Assets\Materials\Mat_FlatColour.h"
#include "Flow\Assets\Materials\Temporary\Mat_Hat_FancyMan.h"
#include "Flow\Assets\Materials\Temporary\Mat_WabbleProps.h"
#include "Flow\Assets\Materials\Temporary\Mat_TexturedPhong.h"

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
		//AssetSystem::LoadAsset("Box2", "Flow/Assets/Models/Box2.obj");
		AssetSystem::LoadAsset("Box", "Flow/Assets/Models/Box.obj");
		//AssetSystem::LoadAsset("WeirdBox", "Flow/Assets/Models/WeirdBox.obj"); 
		AssetSystem::LoadAsset("Hat_FancyMan", "Flow/Assets/Models/Hat_FancyMan.obj");
		//AssetSystem::LoadAsset("Plane", "Flow/Assets/Models/Plane.obj");
		//AssetSystem::LoadAsset("Train", "Flow/Assets/Models/Train.FBX");
		AssetSystem::LoadAsset("Hat_Sherif", "Flow/Assets/Models/Hat_Sherif.obj");
		AssetSystem::LoadAsset("Sphere", "Flow/Assets/Models/Sphere.obj");
		AssetSystem::LoadAsset("SelectionGizmo", "Flow/Assets/Models/SelectionGizmo.obj");
		AssetSystem::LoadAsset("Wabble_Crate", "Flow/Assets/Models/Wabble_Crate.obj");
		AssetSystem::LoadAsset("Wabble_Chair", "Flow/Assets/Models/Wabble_Chair.obj");
		AssetSystem::LoadAsset("Wabble_Table", "Flow/Assets/Models/Wabble_Table.obj");
		AssetSystem::LoadAsset("Wabble_Revolver", "Flow/Assets/Models/Wabble_Revolver.obj");
		AssetSystem::LoadAsset("Wabble_Rifle", "Flow/Assets/Models/Wabble_Rifle.obj");
		AssetSystem::LoadAsset("Wabble_Shotgun", "Flow/Assets/Models/Wabble_Shotgun.obj");
		AssetSystem::LoadAsset("Wabble_Blunderbuss", "Flow/Assets/Models/Wabble_Blunderbuss.obj");
		
		//= Collision ======================

		AssetSystem::LoadAsset("Wabble_Table_Collision", "Flow/Assets/Collision/Collision_Wabble_Table.obj");
		AssetSystem::LoadAsset("Wabble_Chair_Collision", "Flow/Assets/Collision/Collision_Wabble_Chair.obj");
		AssetSystem::LoadAsset("Wabble_Shotgun_Collision", "Flow/Assets/Collision/Collision_Wabble_Shotgun.obj");
		AssetSystem::LoadAsset("Wabble_Rifle_Collision", "Flow/Assets/Collision/Collision_Wabble_Rifle.obj");
		AssetSystem::LoadAsset("Wabble_Revolver_Collision", "Flow/Assets/Collision/Collision_Wabble_Revolver.obj");
		AssetSystem::LoadAsset("Wabble_Blunderbuss_Collision", "Flow/Assets/Collision/Collision_Wabble_Blunderbuss.obj");
		AssetSystem::LoadAsset("Wabble_Crate_Collision", "Flow/Assets/Collision/Collision_Wabble_Crate.obj");


		//Assign Collisions to meshes

		AssetSystem::GetAsset<MeshAsset>("Wabble_Table")->GetMesh(0)->CollisionName_ = "Wabble_Table_Collision";
		AssetSystem::GetAsset<MeshAsset>("Wabble_Chair")->GetMesh(0)->CollisionName_ = "Wabble_Chair_Collision";
		AssetSystem::GetAsset<MeshAsset>("Wabble_Shotgun")->GetMesh(0)->CollisionName_ = "Wabble_Shotgun_Collision";
		AssetSystem::GetAsset<MeshAsset>("Wabble_Rifle")->GetMesh(0)->CollisionName_ = "Wabble_Rifle_Collision";
		AssetSystem::GetAsset<MeshAsset>("Wabble_Revolver")->GetMesh(0)->CollisionName_ = "Wabble_Revolver_Collision";
		AssetSystem::GetAsset<MeshAsset>("Wabble_Blunderbuss")->GetMesh(0)->CollisionName_ = "Wabble_Blunderbuss_Collision";
		AssetSystem::GetAsset<MeshAsset>("Wabble_Crate")->GetMesh(0)->CollisionName_ = "Wabble_Crate_Collision";

		//= Textures =
		//AssetSystem::LoadAsset("ExampleRed", "Flow/Assets/Textures/ExampleRed.png");
		//AssetSystem::LoadAsset("TestTexture", "Flow/Assets/Textures/TestTexture.png");
		//AssetSystem::LoadAsset("TestTextureFlip", "Flow/Assets/Textures/TestTextureFlip.png");
		AssetSystem::LoadAsset("CharacterTexture", "Flow/Assets/Textures/CharacterTexture.png"); 
		AssetSystem::LoadAsset("Wabble_Props", "Flow/Assets/Textures/Wabble_Props.png"); 
		AssetSystem::LoadAsset("Wabble_Weapons", "Flow/Assets/Textures/Wabble_Weapons.png"); 
		AssetSystem::LoadAsset("Wabble_Wood", "Flow/Assets/Textures/Wabble_Wood.png"); 
		AssetSystem::LoadAsset("Wabble_Sand", "Flow/Assets/Textures/Wabble_Sand.png"); 
		//AssetSystem::LoadAsset("TrainTex", "Flow/Assets/Textures/TrainTexture.png"); 

		//= Shaders =
		AssetSystem::LoadAsset("TexturedLightVS", "Flow/Source/Flow/Rendering/Core/Shaders/TexturedPerPixelVS.cso");
		AssetSystem::LoadAsset("TexturedLightPS", "Flow/Source/Flow/Rendering/Core/Shaders/TexturedPerPixelPS.cso");
		AssetSystem::LoadAsset("TexturedPhongPS", "Flow/Source/Flow/Rendering/Core/Shaders/TexturedPhongPS.cso");
		AssetSystem::LoadAsset("TexturedPhongVS", "Flow/Source/Flow/Rendering/Core/Shaders/TexturedPhongVS.cso");
		AssetSystem::LoadAsset("TexturedVS", "Flow/Source/Flow/Rendering/Core/Shaders/TextureVS.cso");
		AssetSystem::LoadAsset("TexturedPS", "Flow/Source/Flow/Rendering/Core/Shaders/TexturePS.cso");
		AssetSystem::LoadAsset("SolidColourVS", "Flow/Source/Flow/Rendering/Core/Shaders/SolidColorVS.cso");
		AssetSystem::LoadAsset("SolidColourPS", "Flow/Source/Flow/Rendering/Core/Shaders/SolidColourPS.cso");
		AssetSystem::LoadAsset("LineShaderP", "Flow/Source/Flow/Rendering/Core/Shaders/LineShaderP.cso");
		AssetSystem::LoadAsset("LineShaderV", "Flow/Source/Flow/Rendering/Core/Shaders/LineShaderV.cso");

		//= Materials =
		AssetSystem::CreateMaterial<Mat_FlatColour>("Mat_FlatColour");
		AssetSystem::CreateMaterial<Mat_FlatColour>("Mat_FlatColour_Brown");
		AssetSystem::CreateMaterial<Mat_Hat_FancyMan>("Mat_HatFancyMan");
		AssetSystem::CreateMaterial<Mat_WabbleProps>("Mat_Wabble_Props");

		AssetSystem::CreateMaterial<Mat_TexturedPhong>("Mat_Wabble_Props2");
		Mat_TexturedPhong* PropsMat = static_cast<Mat_TexturedPhong*>(AssetSystem::GetAsset<MaterialAsset>("Mat_Wabble_Props2")->GetMaterial());
		PropsMat->SetTexture("Wabble_Props");
		PropsMat->SetPixelShader("TexturedPhongPS");
		PropsMat->SetVertexShader("TexturedPhongVS");

		AssetSystem::CreateMaterial<Mat_TexturedPhong>("Mat_Wabble_Weapons");
		Mat_TexturedPhong* WeaponsMat = static_cast<Mat_TexturedPhong*>(AssetSystem::GetAsset<MaterialAsset>("Mat_Wabble_Weapons")->GetMaterial());
		WeaponsMat->SetTexture("Wabble_Weapons");
		WeaponsMat->SetPixelShader("TexturedPhongPS");
		WeaponsMat->SetVertexShader("TexturedPhongVS");

		AssetSystem::CreateMaterial<Mat_TexturedPhong>("Mat_Wood");
		Mat_TexturedPhong* WoodMat = static_cast<Mat_TexturedPhong*>(AssetSystem::GetAsset<MaterialAsset>("Mat_Wood")->GetMaterial());
		WoodMat->SetTexture("Wabble_Wood");
		WoodMat->SetPixelShader("TexturedPhongPS");
		WoodMat->SetVertexShader("TexturedPhongVS");

		AssetSystem::CreateMaterial<Mat_TexturedPhong>("Mat_Sand");
		Mat_TexturedPhong* SandMat = static_cast<Mat_TexturedPhong*>(AssetSystem::GetAsset<MaterialAsset>("Mat_Sand")->GetMaterial());
		SandMat->SetTexture("Wabble_Sand");
		SandMat->SetPixelShader("TexturedPhongPS");
		SandMat->SetVertexShader("TexturedPhongVS");

		static_cast<Mat_FlatColour*>(AssetSystem::GetAsset<MaterialAsset>("Mat_FlatColour_Brown")->GetMaterial())->SetColour(Vector(0.31, 0.08, 0));

		//Create the game world
		GameWorld = new World("Game World");

		m_SelectionGizmo = new SelectionGizmo();
		m_SelectionGizmo->GenerateCollision();
		//m_SelectionGizmo->AddCollidersToWorld(GameWorld);

		m_Inspector = new Inspector(m_SelectionGizmo);


		m_EditorLayer = new EditorLayer();
		PushLayer(m_EditorLayer);
	}

	Application::~Application()
	{
		FLOW_ENGINE_LOG("Shutting Down Engine");

		RenderCommand::Shutdown();
		AssetSystem::Shutdown();
	}

	void Application::Run()
	{
		GameWorld->InitialiseWorld();
		m_Inspector->SetCurrentWorld(GameWorld);

		m_SelectionGizmo->GenerateCollision();
		m_SelectionGizmo->AddCollidersToWorld(GameWorld);

		GameWorld->DispatchBeginPlay();

		while (bRunning)
		{
			float DeltaTime = m_Timer.Mark();

			MainWindow->PreUpdate();
			MainWindow->OnUpdate();

			if (!bPaused)
			{
				//TODO: Check where to move the world since I'm using layers
				GameWorld->Tick(DeltaTime);
			}

			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate(DeltaTime);
			}

			//= Debug Rendering =

			if(DrawCollision_)
				GameWorld->GetPhysicsWorld()->debugDrawWorld();

			//= UI Rendering =

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
			{
				layer->OnImGuiRender();
			}

			m_Inspector->Update();

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

	Inspector* Application::GetInspector()
	{
		return m_Inspector;
	}

	Window& Application::GetWindow()
	{
		return *MainWindow;
	}

	void Application::RenderApplicationDebug(float DeltaTime)
	{
		TimeSinceFrameRateCheck += DeltaTime;
		FrameCounter++;

		if (FrameCounter == 5)
		{
			FrameTimer = TimeSinceFrameRateCheck / 5;
			TimeSinceFrameRateCheck = 0;
			FrameCounter = 0;
		}	

		if (ImGui::Begin("Application Statistics"))
		{
			ImGui::Checkbox("Pause Game", &bPaused);
			ImGui::Checkbox("Draw Collision", &DrawCollision_);

			ImGui::Text("Framerate: %.1f", 1 / FrameTimer);
			ImGui::Text("FrameTime: %.1f ms", DeltaTime * 1000);

			PROCESS_MEMORY_COUNTERS MemoryData;
			GetProcessMemoryInfo(GetCurrentProcess(), &MemoryData, sizeof(MemoryData));

			ImGui::Text("Memory: %.1f MB", (float)MemoryData.WorkingSetSize / 1048576);
		}
		ImGui::End();
	}
}
