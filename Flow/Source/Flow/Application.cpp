#include "Flowpch.h"
#include "Application.h"
#include "Logging/Log.h"
#include "Events/ApplicationEvent.h"

#include "Flow/Input/Input.h"
#include "ThirdParty\ImGui\imgui.h"

#include "Flow\Assets\AssetSystem.h"

#include "Flow\GameFramework\World.h"

#if WITH_EDITOR

#include "Flow\Editor\Inspector.h"
#include "Flow\Layers\EditorLayer.h"

#endif

//TODO: Load somewhere else
#include "Flow\Assets\Materials\Mat_FlatColour.h"
#include "Flow\Assets\Materials\Mat_TexturedPhong.h"
#include "Flow\Assets\Meshes\MeshAsset.h"

#include "Flow\Helper\Profiling.h"

#define BIND_EVENT_FUNCTION(FunctionPtr) std::bind(FunctionPtr, this, std::placeholders::_1)

Application* Application::s_Instance = nullptr;

Application::Application(const std::string& AppName)
	: ApplicationName(AppName)
{
	
}

void Application::InitialiseApplication()
{
	Instrumentor::Get().BeginSession("Application Startup", "Saved/Profiling-Startup.json");
	PROFILE_FUNCTION();

	s_Instance = this;

	_MainWindow = std::unique_ptr<Window>(Window::Create(WindowProperties(ApplicationName, 1280u, 720u)));
	_MainWindow->SetEventCallback(BIND_EVENT_FUNCTION(&Application::OnEvent));

	ImGuiLayer_ = new ImGuiLayer();
	PushOverlay(ImGuiLayer_);

	//Get Local File Path
	char Path[128];
	GetModuleFileName(nullptr, Path, sizeof(Path));
	std::string ExeDir = std::string(Path);
	LocalPath_ = ExeDir.substr(0, ExeDir.find("bin"));

	//TODO: Load assets somewhere
	//= Models =

	AssetSystem::LoadAsset("Box", "Flow/Assets/Models/Box.obj");
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

	AssetSystem::GetAsset<MeshAsset>("Wabble_Table")->GetMesh(0)->_CollisionName = "Wabble_Table_Collision";
	AssetSystem::GetAsset<MeshAsset>("Wabble_Chair")->GetMesh(0)->_CollisionName = "Wabble_Chair_Collision";
	AssetSystem::GetAsset<MeshAsset>("Wabble_Shotgun")->GetMesh(0)->_CollisionName = "Wabble_Shotgun_Collision";
	AssetSystem::GetAsset<MeshAsset>("Wabble_Rifle")->GetMesh(0)->_CollisionName = "Wabble_Rifle_Collision";
	AssetSystem::GetAsset<MeshAsset>("Wabble_Revolver")->GetMesh(0)->_CollisionName = "Wabble_Revolver_Collision";
	AssetSystem::GetAsset<MeshAsset>("Wabble_Blunderbuss")->GetMesh(0)->_CollisionName = "Wabble_Blunderbuss_Collision";
	AssetSystem::GetAsset<MeshAsset>("Wabble_Crate")->GetMesh(0)->_CollisionName = "Wabble_Crate_Collision";

	//= Textures =

	AssetSystem::LoadAsset("Wabble_Props", "Flow/Assets/Textures/Wabble_Props.png");
	AssetSystem::LoadAsset("Wabble_Weapons", "Flow/Assets/Textures/Wabble_Weapons.png");
	AssetSystem::LoadAsset("Wabble_Wood", "Flow/Assets/Textures/Wabble_Wood.png");
	AssetSystem::LoadAsset("Wabble_Sand", "Flow/Assets/Textures/Wabble_Sand.png");
	AssetSystem::LoadAsset("SkyCube_Test", "Flow/Assets/Textures/TestCubeMap2.png");


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
	AssetSystem::LoadAsset("LineColourP", "Flow/Source/Flow/Rendering/Core/Shaders/LineColour_PS.cso");
	AssetSystem::LoadAsset("LineColourV", "Flow/Source/Flow/Rendering/Core/Shaders/LineColour_VS.cso");

	//= Materials =
	AssetSystem::CreateMaterial<Mat_FlatColour>("Mat_FlatColour");
	AssetSystem::CreateMaterial<Mat_FlatColour>("Mat_FlatColour_Brown");
	static_cast<Mat_FlatColour*>(AssetSystem::GetAsset<MaterialAsset>("Mat_FlatColour_Brown")->GetMaterial())->SetColour(Vector(0.31, 0.08, 0));
	AssetSystem::CreateMaterial<Mat_FlatColour>("Mat_FlatColour_White");
	static_cast<Mat_FlatColour*>(AssetSystem::GetAsset<MaterialAsset>("Mat_FlatColour_White")->GetMaterial())->SetColour(Vector(1.0f, 1.0f, 1.0f));
	AssetSystem::CreateMaterial<Mat_FlatColour>("Mat_FlatColour_Blue");
	static_cast<Mat_FlatColour*>(AssetSystem::GetAsset<MaterialAsset>("Mat_FlatColour_Blue")->GetMaterial())->SetColour(Vector(0.52f, 0.8f, 1.0f));

	AssetSystem::CreateMaterial<Mat_TexturedPhong>("Mat_Wabble_Props");
	Mat_TexturedPhong* PropsMat = static_cast<Mat_TexturedPhong*>(AssetSystem::GetAsset<MaterialAsset>("Mat_Wabble_Props")->GetMaterial());
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

	AssetSystem::CreateMaterial<Mat_TexturedPhong>("Mat_SkyCube");
	Mat_TexturedPhong* SkyMat = static_cast<Mat_TexturedPhong*>(AssetSystem::GetAsset<MaterialAsset>("Mat_SkyCube")->GetMaterial());
	SkyMat->SetTexture("SkyCube_Test");
	SkyMat->SetPixelShader("TexturedPS");
	SkyMat->SetVertexShader("TexturedVS");

#if WITH_EDITOR
	//Create the editor
	EditorLayer_ = new EditorLayer();

#endif
	//Create the game world
	GameWorld_ = new World("Game World");

#if WITH_EDITOR
	PushLayer(EditorLayer_);

#endif
}

Application::~Application()
{
	FLOW_ENGINE_LOG("Shutting Down Engine");

	RenderCommand::Shutdown();
	AssetSystem::Shutdown();
}

void Application::Run()
{
	{
		PROFILE_CURRENT_SCOPE("Run Initialisation");

		GameWorld_->InitialiseWorld();

#if WITH_EDITOR
		GameWorld_->StartEditor();
#else
		StartGame();
#endif // WITH_EDITOR



		Timer_.Mark(); // Reset timer to avoid a long initial deltatime
	}

	Instrumentor::Get().EndSession();
	Instrumentor::Get().BeginSession("Game Running", "Saved\\Profiling-GameLoop.json");

	while (_Running)
	{
		PROFILE_CURRENT_SCOPE("Game Loop");

		float DeltaTime = Timer_.Mark();

		_MainWindow->PreUpdate();
		_MainWindow->OnUpdate();

		//If the window has sent a shutdown message return immediately
		if (!_Running)
		{
			//Finish the profiling first
			Instrumentor::Get().EndSession();
			return;
		}

		if (!Paused_)
		{
			PROFILE_CURRENT_SCOPE("Game - Tick");

			//TODO: Check where to move the world since I'm using layers
			GameWorld_->Tick(DeltaTime);
		}

		{
			PROFILE_CURRENT_SCOPE("Game - Layer Updates");

			for (Layer* layer : LayerStack_)
			{
				layer->OnUpdate(DeltaTime);
			}
		}


		//= Debug Rendering =
		{
			PROFILE_CURRENT_SCOPE("Game - Draw Debug Physics");

			if (DrawCollision_)
				GameWorld_->GetPhysicsWorld()->debugDrawWorld();

			GameWorld_->GetLineBatcher().DrawLines();
		}


		//= UI Rendering =
		{
			PROFILE_CURRENT_SCOPE("Game - ImGui Rendering");
			ImGuiLayer_->Begin();
			for (Layer* layer : LayerStack_)
			{
#if WITH_EDITOR
				layer->OnImGuiRender(_DrawEditor);
#else
				layer->OnImGuiRender(false);
#endif
			}
			ImGuiLayer_->End();
		}

		//= Post Update =

		_MainWindow->PostUpdate();

		//Bad way of delaying window destruction, can fix to do it better another time
		_UpdatingChildWindows = true;
		//Update other windows
		for (auto& Window : _Windows)
		{
			Window->PreUpdate();
			Window->OnUpdate();
			Window->PostUpdate();
		}
		_UpdatingChildWindows = false;
		UpdateWindowDestruction();
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
		if (e._Handled)
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
	_Running = false;
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
	return *s_Instance;
}

World* Application::GetWorld()
{
	return s_Instance->GameWorld_;
}

void Application::Shutdown()
{
	Application::GetApplication()._Running = false;
}

void Application::SaveLevel()
{
	Application::GetWorld()->Save();
}

void Application::LoadLevel()
{
	Application::GetWorld()->Load();
}

std::string Application::GetLocalFilePath()
{
	return LocalPath_;
}

std::wstring Application::GetLocalFilePathWide()
{
	return std::wstring(LocalPath_.begin(), LocalPath_.end());
}

Window* Application::CreateNewWindow(const std::string& WindowName)
{
	Application& App = Application::GetApplication();

	Window* Win = Window::Create(WindowProperties(WindowName, 1280u, 720u), false);
	App._Windows.push_back(Win);
	return Win;
}

bool Application::RegisterWindow(Window* NewWindow)
{
	Application& App = Application::GetApplication();
	App._Windows.push_back(NewWindow);
	return true;
}

bool Application::DeRegisterWindow(Window* Win)
{
	Application& App = Application::GetApplication();
	if (App._UpdatingChildWindows)
	{
		App._WindowsToDestroy.push_back(Win);
		return false;
	}

	auto FindIt = std::find(App._Windows.begin(), App._Windows.end(), Win);

	if (FindIt == App._Windows.end())
	{
		FLOW_ENGINE_WARNING("Application::DeRegisterWindow: Tried to get rid of window but failed to find it");
		return false;
	}
	
	//Remove and delete it
	Window* FoundWin = *FindIt;
	App._Windows.erase(FindIt);
	delete FoundWin;

	return true;
}

Window& Application::GetWindow()
{
	return *_MainWindow;
}

void Application::StartGame()
{
	Application& App = Application::GetApplication();

	if (App.GameWorld_->GetWorldState() == WorldState::InGame)
		return;

	for (Layer* layer : App.LayerStack_)
	{
		layer->BeginPlay();
	}
	App.GameWorld_->StartGame();
}

void Application::PauseGame()
{
	Application& App = Application::GetApplication();

	if (App.GameWorld_->GetWorldState() != WorldState::InGame)
		return;

	App.GameWorld_->PauseGame();
}

void Application::StopGame()
{
	Application& App = Application::GetApplication();

	if (App.GameWorld_->GetWorldState() != WorldState::InGame)
		return;

	App.GameWorld_->StopGame();
}

void Application::UpdateWindowDestruction()
{
	std::vector<Window*> WindowsRemoved;
	for (auto& Wind : _WindowsToDestroy)
	{
		if (DeRegisterWindow(Wind))
			WindowsRemoved.push_back(Wind);
	}

	//Lazy way
	for (auto& Wind : WindowsRemoved)
	{
		_WindowsToDestroy.erase(std::find(_WindowsToDestroy.begin(), _WindowsToDestroy.end(), Wind));
	}
}


