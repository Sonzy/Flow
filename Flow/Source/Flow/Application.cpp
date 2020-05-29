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
#include "Flow\Assets\Materials\Mat_TexturedPhong.h"
#include "Flow\Assets\Meshes\MeshAsset.h"

#include "Flow\Helper\Instrumentation.h"
#include "Flow\GameFramework\Components\CameraComponent.h"

#define BIND_EVENT_FUNCTION(FunctionPtr) std::bind(FunctionPtr, this, std::placeholders::_1)

namespace Flow
{
	Application* Application::Instance = nullptr;

	Application::Application(const std::string& AppName)
		: ApplicationName(AppName)
	{

	}

	Application::~Application()
	{
		FLOW_ENGINE_LOG("Shutting Down Engine");

		RenderCommand::Shutdown();
		AssetSystem::Shutdown();
	}

	void Application::InitialiseApplication()
	{
		Instrumentor::Get().BeginSession("Application Startup", "Profile-Startup.json");

		PROFILE_FUNCTION();

		Instance = this;

		MainWindow_ = std::unique_ptr<Window>(Window::Create(WindowProperties(ApplicationName, 1280u, 720u)));
		MainWindow_->SetEventCallback(BIND_EVENT_FUNCTION(&Application::OnEvent));
		//std::function<void(Event&)> = ( std::bind(&Application::OnEvent, this, std::placeholders::_1);

		ImGuiLayer_ = new ImGuiLayer();
		PushOverlay(ImGuiLayer_);

		//Get Local File Path
		char Path[128];
		GetModuleFileName(nullptr, Path, sizeof(Path));
		std::string ExeDir = std::string(Path);
		std::string SandboxExe = std::string("Sandbox.exe");


#define FLOW_EXTERNAL_BUILD
#ifdef FLOW_EXTERNAL_BUILD
		LocalPath_ = ExeDir.substr(0, ExeDir.find("Sandbox.exe"));
#else
		LocalPath_ = ExeDir.substr(0, ExeDir.find("bin"));
#endif // FLOW_EXTERNAL_BUILD

		FLOW_ENGINE_LOG("RootPath is: {0}", LocalPath_);
		FLOW_ENGINE_LOG("INITIALISING");

		//TODO: Load assets somewhere
		//= Models =
		//AssetSystem::LoadAsset("Box2", "Flow/Assets/Models/Box2.obj");
		AssetSystem::LoadAsset("Box", "Flow/Assets/Models/Box1m.obj");
		////AssetSystem::LoadAsset("WeirdBox", "Flow/Assets/Models/WeirdBox.obj"); 
		//AssetSystem::LoadAsset("Hat_FancyMan", "Flow/Assets/Models/Hat_FancyMan.obj");
		////AssetSystem::LoadAsset("Plane", "Flow/Assets/Models/Plane.obj");
		////AssetSystem::LoadAsset("Train", "Flow/Assets/Models/Train.FBX");
		//AssetSystem::LoadAsset("Hat_Sherif", "Flow/Assets/Models/Hat_Sherif.obj");
		//AssetSystem::LoadAsset("Sphere", "Flow/Assets/Models/Sphere.obj");
		//AssetSystem::LoadAsset("SelectionGizmo", "Flow/Assets/Models/SelectionGizmo.obj");
		//AssetSystem::LoadAsset("Wabble_Crate", "Flow/Assets/Models/Wabble_Crate.obj");
		//AssetSystem::LoadAsset("Wabble_Chair", "Flow/Assets/Models/Wabble_Chair.obj");
		//AssetSystem::LoadAsset("Wabble_Table", "Flow/Assets/Models/Wabble_Table.obj");
		//AssetSystem::LoadAsset("Wabble_Revolver", "Flow/Assets/Models/Wabble_Revolver.obj");
		//AssetSystem::LoadAsset("Wabble_Rifle", "Flow/Assets/Models/Wabble_Rifle.obj");
		//AssetSystem::LoadAsset("Wabble_Shotgun", "Flow/Assets/Models/Wabble_Shotgun.obj");
		//AssetSystem::LoadAsset("Wabble_Blunderbuss", "Flow/Assets/Models/Wabble_Blunderbuss.obj");

		//= Collision ======================

		//AssetSystem::LoadAsset("Wabble_Table_Collision", "Flow/Assets/Collision/Collision_Wabble_Table.obj");
		//AssetSystem::LoadAsset("Wabble_Chair_Collision", "Flow/Assets/Collision/Collision_Wabble_Chair.obj");
		//AssetSystem::LoadAsset("Wabble_Shotgun_Collision", "Flow/Assets/Collision/Collision_Wabble_Shotgun.obj");
		//AssetSystem::LoadAsset("Wabble_Rifle_Collision", "Flow/Assets/Collision/Collision_Wabble_Rifle.obj");
		//AssetSystem::LoadAsset("Wabble_Revolver_Collision", "Flow/Assets/Collision/Collision_Wabble_Revolver.obj");
		//AssetSystem::LoadAsset("Wabble_Blunderbuss_Collision", "Flow/Assets/Collision/Collision_Wabble_Blunderbuss.obj");
		//AssetSystem::LoadAsset("Wabble_Crate_Collision", "Flow/Assets/Collision/Collision_Wabble_Crate.obj");


		//Assign Collisions to meshes

		//AssetSystem::GetAsset<MeshAsset>("Wabble_Table")->GetMesh(0)->CollisionName_ = "Wabble_Table_Collision";
		//AssetSystem::GetAsset<MeshAsset>("Wabble_Chair")->GetMesh(0)->CollisionName_ = "Wabble_Chair_Collision";
		//AssetSystem::GetAsset<MeshAsset>("Wabble_Shotgun")->GetMesh(0)->CollisionName_ = "Wabble_Shotgun_Collision";
		//AssetSystem::GetAsset<MeshAsset>("Wabble_Rifle")->GetMesh(0)->CollisionName_ = "Wabble_Rifle_Collision";
		//AssetSystem::GetAsset<MeshAsset>("Wabble_Revolver")->GetMesh(0)->CollisionName_ = "Wabble_Revolver_Collision";
		//AssetSystem::GetAsset<MeshAsset>("Wabble_Blunderbuss")->GetMesh(0)->CollisionName_ = "Wabble_Blunderbuss_Collision";
		//AssetSystem::GetAsset<MeshAsset>("Wabble_Crate")->GetMesh(0)->CollisionName_ = "Wabble_Crate_Collision";

		//= Textures =
		//AssetSystem::LoadAsset("ExampleRed", "Flow/Assets/Textures/ExampleRed.png");
		//AssetSystem::LoadAsset("TestTexture", "Flow/Assets/Textures/TestTexture.png");
		//AssetSystem::LoadAsset("TestTextureFlip", "Flow/Assets/Textures/TestTextureFlip.png");
		//AssetSystem::LoadAsset("CharacterTexture", "Flow/Assets/Textures/CharacterTexture.png"); 
		//AssetSystem::LoadAsset("Wabble_Props", "Flow/Assets/Textures/Wabble_Props.png"); 
		//AssetSystem::LoadAsset("Wabble_Weapons", "Flow/Assets/Textures/Wabble_Weapons.png"); 
		AssetSystem::LoadAsset("Wabble_Wood", "Flow/Assets/Textures/Wabble_Wood.png"); 
		//AssetSystem::LoadAsset("Wabble_Sand", "Flow/Assets/Textures/Wabble_Sand.png"); 
		AssetSystem::LoadAsset("SkyCube_Test", "Flow/Assets/Textures/TestCubeMap2.png");
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
		AssetSystem::LoadAsset("LineShaderV", "Flow/Source/Flow/Rendering/Core/Shaders/LineShaderV.cso");		AssetSystem::LoadAsset("LineShaderP", "Flow/Source/Flow/Rendering/Core/Shaders/LineShaderP.cso");
		AssetSystem::LoadAsset("LineColourP", "Flow/Source/Flow/Rendering/Core/Shaders/LineColourP.cso");
		AssetSystem::LoadAsset("LineColourV", "Flow/Source/Flow/Rendering/Core/Shaders/LineColourV.cso");

		//= Materials =
		AssetSystem::CreateMaterial<Mat_FlatColour>("Mat_FlatColour");
		AssetSystem::CreateMaterial<Mat_FlatColour>("Mat_FlatColour_Brown");
		static_cast<Mat_FlatColour*>(AssetSystem::GetAsset<MaterialAsset>("Mat_FlatColour_Brown")->GetMaterial())->SetColour(Vector(0.31, 0.08, 0));
		AssetSystem::CreateMaterial<Mat_FlatColour>("Mat_FlatColour_White");
		static_cast<Mat_FlatColour*>(AssetSystem::GetAsset<MaterialAsset>("Mat_FlatColour_White")->GetMaterial())->SetColour(Vector(1.0f, 1.0f, 1.0f));
		AssetSystem::CreateMaterial<Mat_FlatColour>("Mat_FlatColour_Blue");
		static_cast<Mat_FlatColour*>(AssetSystem::GetAsset<MaterialAsset>("Mat_FlatColour_Blue")->GetMaterial())->SetColour(Vector(0.52f, 0.8f, 1.0f));


		AssetSystem::CreateMaterial<Mat_TexturedPhong>("Mat_SkyCube");
		Mat_TexturedPhong* SkyMat = static_cast<Mat_TexturedPhong*>(AssetSystem::GetAsset<MaterialAsset>("Mat_SkyCube")->GetMaterial());
		SkyMat->SetTexture("SkyCube_Test");
		SkyMat->SetPixelShader("TexturedPS");
		SkyMat->SetVertexShader("TexturedVS");

		//Create the game world
		GameWorld_ = new World("Game World");

		//SelectionGizmo_ = new SelectionGizmo();
		//SelectionGizmo_->GenerateCollision();
		//SelectionGizmo_->AddCollidersToWorld(GameWorld_);

		Inspector_ = new Inspector(SelectionGizmo_);


		EditorLayer_ = new EditorLayer();
		PushLayer(EditorLayer_);
	}

	void Application::Run()
	{
		{
			PROFILE_CURRENT_SCOPE("Run Initialisation");

			GameWorld_->InitialiseWorld();
			Inspector_->SetCurrentWorld(GameWorld_);

			//SelectionGizmo_->GenerateCollision();
			//SelectionGizmo_->AddCollidersToWorld(GameWorld_);

			GameWorld_->DispatchBeginPlay();

			Timer_.Mark(); // Reset timer to avoid a long initial deltatime

			for (Layer* layer : LayerStack_)
			{
				layer->PostBeginPlay();
			}
		}


		Instrumentor::Get().EndSession();
		Instrumentor::Get().BeginSession("Game Running", "Profile-GameLoop.json");
		while (Running_)
		{
			PROFILE_CURRENT_SCOPE("Application Loop");

			float DeltaTime = Timer_.Mark();

			MainWindow_->PreUpdate();
			MainWindow_->OnUpdate();

			//If the window has sent a shutdown message return immediately
			if (!Running_)
			{
				//Finish the profiling first
				Instrumentor::Get().EndSession();
				return;
			}
				

			if (!Paused_)
			{
				//TODO: Check where to move the world since I'm using layers
				GameWorld_->Tick(DeltaTime);

				RenderCommand::GetCamera().Update(DeltaTime);
			}

			{
				PROFILE_CURRENT_SCOPE("Layer Updates");

				for (Layer* layer : LayerStack_)
				{
					layer->OnUpdate(DeltaTime);
				}
			}


			//= Debug Rendering =
			
			{
				PROFILE_CURRENT_SCOPE("Construct Debug World");

				if (DrawCollision_)
				{
					Line::Count = 0;
 					GameWorld_->GetPhysicsWorld()->debugDrawWorld();
				}
			}


			{
				PROFILE_CURRENT_SCOPE("Draw Debug World");

				GameWorld_->DrawAllLines();
				//RenderCommand::DrawIndexed(Line::IndexBufferCount * Line::Count);
			}
	

			//= UI Rendering =

			{
				PROFILE_CURRENT_SCOPE("IMGUI Rendering");

				ImGuiLayer_->Begin();
				UIConfigRender();
				if (RenderUI)
				{
					for (Layer* layer : LayerStack_)
					{
						layer->OnImGuiRender();
					}

					Inspector_->Update();

					RenderApplicationDebug(DeltaTime);
				}
				ImGuiLayer_->End();
			}


			//= Post Update =

			RenderCommand::GetCamera().MarkCacheDirty();
			MainWindow_->PostUpdate();
		}

		Instrumentor::Get().EndSession();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher Dispatcher(e);
		Dispatcher.Dispatch<WindowClosedEvent>(BIND_EVENT_FUNCTION(&Application::OnWindowClosed));
		Dispatcher.Dispatch<WindowResizedEvent>(BIND_EVENT_FUNCTION(&Application::OnWindowResized));

		for (auto iterator = LayerStack_.end(); iterator != LayerStack_.begin();)
		{
			(*--iterator)->OnEvent(e);
			if (e.Handled_)
				break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		LayerStack_.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		LayerStack_.PushOverlay(layer);
		layer->OnAttach();
	}

	bool Application::OnWindowClosed(WindowClosedEvent& e)
	{
		Running_ = false;
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
		return Instance->GameWorld_;
	}

	std::string Application::GetLocalFilePath()
	{
		return LocalPath_;
	}

	std::wstring Application::GetLocalFilePathWide()
	{
		return std::wstring(LocalPath_.begin(), LocalPath_.end());
	}

	Inspector* Application::GetInspector()
	{
		return Inspector_;
	}

	Window& Application::GetWindow()
	{
		return *MainWindow_;
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
			ImGui::Checkbox("Pause Game", &Paused_);
			ImGui::Checkbox("Draw Collision", &DrawCollision_);

			ImGui::Text("Framerate: %.1f", 1 / FrameTimer);
			ImGui::Text("FrameTime: %.1f ms", DeltaTime * 1000);

			PROCESS_MEMORY_COUNTERS MemoryData;
			GetProcessMemoryInfo(GetCurrentProcess(), &MemoryData, sizeof(MemoryData));

			ImGui::Text("Memory: %.1f MB", (float)MemoryData.WorkingSetSize / 1048576);
		}
		ImGui::End();
	}

	void Application::UIConfigRender()
	{
		if(ImGui::Begin("UI Config"))
		{
			ImGui::Checkbox("Render Editor UI", &RenderUI);
		}
		ImGui::End();
	}
}
