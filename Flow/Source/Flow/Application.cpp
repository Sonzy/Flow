#include "Flowpch.h"
#include "Application.h"

//= Misc Includes =
#include "Logging/Log.h"


//= Input Includes =
#include "Events/ApplicationEvent.h"
#include "Flow/Input/Input.h"
#include "ThirdParty\ImGui\imgui.h"

//= Editor Includes =
#if WITH_EDITOR
#include "Flow\Editor\Inspector.h"
#include "Flow/Editor/Editor.h"
#endif

//= Asset Includes =
#include "Flow\Assets\AssetSystem.h"
#include "Flow\Assets\Materials\Mat_FlatColour.h"
#include "Flow\Assets\Materials\Mat_TexturedPhong.h"
#include "Flow\Assets\Meshes\MeshAsset.h"

//= Helper Inclues =
#include "Flow\Utils\Profiling.h"
#include "Flow/GameFramework/Other/ClassFactory.h"

//= Game Framework includes =
#include "Flow\GameFramework\World.h"
#include "Flow/Layers/GameLayer.h"

#define BIND_EVENT_FUNCTION(FunctionPtr) std::bind(FunctionPtr, this, std::placeholders::_1)


//= Static Variable Definitions ======================================
Application* Application::sm_Application = nullptr;

//TODO: Being naughty, need to null initialise
Application::Application(const std::string& AppName)
	: m_ApplicationName(AppName)
	, m_MainWindow(nullptr)
	, m_Layer_ImGui(nullptr)
	, m_Layer_Game(nullptr)
	, m_Running(true)
	, m_GamePaused(false)
	, m_DrawCollision(false)
	, m_GameWorld(nullptr)
	, m_ClassFactory(nullptr)
	, m_ApplicationPath()

#if WITH_EDITOR
	, m_RenderEditor(true)
	, m_Layer_Editor(nullptr)
#endif

{
}

void Application::InitialiseApplication()
{
	Instrumentor::Get().BeginSession("Application Startup", "Saved/Profiling-Startup.json");

	PROFILE_FUNCTION();

	sm_Application = this;
	m_ApplicationPath = std::filesystem::current_path();

	//Create the main window
	m_MainWindow = Window::Create(Window::Properties(m_ApplicationName, 1280u, 720u));
	m_MainWindow->SetEventCallback(BIND_EVENT_FUNCTION(&Application::OnEvent));

	//Initialise the debug UI Layer
	m_Layer_ImGui = new ImGuiLayer();
	PushOverlay(m_Layer_ImGui);

	//= Build class factory map before the application starts
	m_ClassFactory = new ClassFactory();
	m_ClassFactory->RegisterClassUIDs();

	//TODO: Load assets somewhere
	//= Models =

	AssetSystem::LoadEditorAsset("Box",						"Assets/Models/Box.obj");
	AssetSystem::LoadEditorAsset("Arrow",					"Assets/Models/Arrow.obj");
	AssetSystem::LoadEditorAsset("Gizmo_Translation",		"Assets/Models/Gizmo_Translation.obj"); //TODO: we load it as a seperate asset for laziness
	AssetSystem::LoadEditorAsset("Gizmo_Rotation",			"Assets/Models/Gizmo_Rotation.obj");
	AssetSystem::LoadEditorAsset("Gizmo_Scale",				"Assets/Models/Gizmo_Scale.obj");
	AssetSystem::LoadEditorAsset("Wabble_Crate",			"Assets/Models/Wabble_Crate.obj");
	AssetSystem::LoadEditorAsset("Wabble_Chair",			"Assets/Models/Wabble_Chair.obj");
	AssetSystem::LoadEditorAsset("Wabble_Table",			"Assets/Models/Wabble_Table.obj");
	AssetSystem::LoadEditorAsset("Wabble_Revolver",			"Assets/Models/Wabble_Revolver.obj");
	AssetSystem::LoadEditorAsset("Wabble_Rifle",			"Assets/Models/Wabble_Rifle.obj");
	AssetSystem::LoadEditorAsset("Wabble_Shotgun",			"Assets/Models/Wabble_Shotgun.obj");
	AssetSystem::LoadEditorAsset("Wabble_Blunderbuss",		"Assets/Models/Wabble_Blunderbuss.obj");

	//= Collision ======================

	AssetSystem::LoadEditorAsset("Wabble_Table_Collision",			"Assets/Collision/Collision_Wabble_Table.obj");
	AssetSystem::LoadEditorAsset("Wabble_Chair_Collision",			"Assets/Collision/Collision_Wabble_Chair.obj");
	AssetSystem::LoadEditorAsset("Wabble_Shotgun_Collision",		"Assets/Collision/Collision_Wabble_Shotgun.obj");
	AssetSystem::LoadEditorAsset("Wabble_Rifle_Collision",			"Assets/Collision/Collision_Wabble_Rifle.obj");
	AssetSystem::LoadEditorAsset("Wabble_Revolver_Collision",		"Assets/Collision/Collision_Wabble_Revolver.obj");
	AssetSystem::LoadEditorAsset("Wabble_Blunderbuss_Collision",	"Assets/Collision/Collision_Wabble_Blunderbuss.obj");
	AssetSystem::LoadEditorAsset("Wabble_Crate_Collision",			"Assets/Collision/Collision_Wabble_Crate.obj");


	//Assign Collisions to meshes

	AssetSystem::GetAsset<MeshAsset>("Wabble_Table")->GetMesh(0)->m_CollisionName = "Wabble_Table_Collision";
	AssetSystem::GetAsset<MeshAsset>("Wabble_Chair")->GetMesh(0)->m_CollisionName = "Wabble_Chair_Collision";
	AssetSystem::GetAsset<MeshAsset>("Wabble_Shotgun")->GetMesh(0)->m_CollisionName = "Wabble_Shotgun_Collision";
	AssetSystem::GetAsset<MeshAsset>("Wabble_Rifle")->GetMesh(0)->m_CollisionName = "Wabble_Rifle_Collision";
	AssetSystem::GetAsset<MeshAsset>("Wabble_Revolver")->GetMesh(0)->m_CollisionName = "Wabble_Revolver_Collision";
	AssetSystem::GetAsset<MeshAsset>("Wabble_Blunderbuss")->GetMesh(0)->m_CollisionName = "Wabble_Blunderbuss_Collision";
	AssetSystem::GetAsset<MeshAsset>("Wabble_Crate")->GetMesh(0)->m_CollisionName = "Wabble_Crate_Collision";

	//= Textures =

	AssetSystem::LoadEditorAsset("Wabble_Props", "Assets/Textures/Wabble_Props.png");
	AssetSystem::LoadEditorAsset("Wabble_Weapons", "Assets/Textures/Wabble_Weapons.png");
	AssetSystem::LoadEditorAsset("Wabble_Wood", "Assets/Textures/Wabble_Wood.png");
	AssetSystem::LoadEditorAsset("Wabble_Sand", "Assets/Textures/Wabble_Sand.png");
	AssetSystem::LoadEditorAsset("SkyCube_Test", "Assets/Textures/TestCubeMap2.png");

	//= Icons =

	AssetSystem::LoadEditorAsset("Icon_SelectionTool", "Assets/Images/SelectionTool.png");
	AssetSystem::LoadEditorAsset("Icon_Play", "Assets/Images/Icon_Play.png");
	AssetSystem::LoadEditorAsset("Icon_Pause", "Assets/Images/Icon_Pause.png");
	AssetSystem::LoadEditorAsset("Icon_Stop", "Assets/Images/Icon_Stop.png");


	//= Shaders =
	AssetSystem::LoadEditorAsset("TexturedLightVS", "Source/Flow/Rendering/Core/Shaders/TexturedPerPixelVS.cso");
	AssetSystem::LoadEditorAsset("TexturedLightPS", "Source/Flow/Rendering/Core/Shaders/TexturedPerPixelPS.cso");
	AssetSystem::LoadEditorAsset("TexturedPhongPS", "Source/Flow/Rendering/Core/Shaders/TexturedPhongPS.cso");
	AssetSystem::LoadEditorAsset("TexturedPhongVS", "Source/Flow/Rendering/Core/Shaders/TexturedPhongVS.cso");
	AssetSystem::LoadEditorAsset("TexturedVS", "Source/Flow/Rendering/Core/Shaders/TextureVS.cso");
	AssetSystem::LoadEditorAsset("TexturedPS", "Source/Flow/Rendering/Core/Shaders/TexturePS.cso");
	AssetSystem::LoadEditorAsset("SolidColourVS", "Source/Flow/Rendering/Core/Shaders/SolidColorVS.cso");
	AssetSystem::LoadEditorAsset("SolidColourPS", "Source/Flow/Rendering/Core/Shaders/SolidColourPS.cso");
	AssetSystem::LoadEditorAsset("LineShaderP", "Source/Flow/Rendering/Core/Shaders/LineShaderP.cso");
	AssetSystem::LoadEditorAsset("LineShaderV", "Source/Flow/Rendering/Core/Shaders/LineShaderV.cso");
	AssetSystem::LoadEditorAsset("LineColourP", "Source/Flow/Rendering/Core/Shaders/LineColour_PS.cso");
	AssetSystem::LoadEditorAsset("LineColourV", "Source/Flow/Rendering/Core/Shaders/LineColour_VS.cso");

	//= Materials =
	AssetSystem::CreateMaterial<Mat_FlatColour>("Mat_FlatColour");
	AssetSystem::CreateMaterial<Mat_FlatColour>("Mat_FlatColour_Brown");
	static_cast<Mat_FlatColour*>(AssetSystem::GetAsset<MaterialAsset>("Mat_FlatColour_Brown")->GetMaterial())->SetColour(Vector3(0.31f, 0.08f, 0.0f));
	AssetSystem::CreateMaterial<Mat_FlatColour>("Mat_FlatColour_White");
	static_cast<Mat_FlatColour*>(AssetSystem::GetAsset<MaterialAsset>("Mat_FlatColour_White")->GetMaterial())->SetColour(Vector3(1.0f, 1.0f, 1.0f));
	AssetSystem::CreateMaterial<Mat_FlatColour>("Mat_FlatColour_Blue");
	static_cast<Mat_FlatColour*>(AssetSystem::GetAsset<MaterialAsset>("Mat_FlatColour_Blue")->GetMaterial())->SetColour(Vector3(0.0f, 0.0f, 1.0f));
	AssetSystem::CreateMaterial<Mat_FlatColour>("Mat_FlatColour_Red");
	static_cast<Mat_FlatColour*>(AssetSystem::GetAsset<MaterialAsset>("Mat_FlatColour_Red")->GetMaterial())->SetColour(Vector3(1.0f, 0.0f, 0.0f));
	AssetSystem::CreateMaterial<Mat_FlatColour>("Mat_FlatColour_Green");
	static_cast<Mat_FlatColour*>(AssetSystem::GetAsset<MaterialAsset>("Mat_FlatColour_Green")->GetMaterial())->SetColour(Vector3(0.0f, 1.0f, 0.0f));

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

	m_Layer_Game = new GameLayer();

	//Create the game world
	m_GameWorld = new World("Game World");

	//Create the editor
#if WITH_EDITOR
	m_Layer_Editor = new Editor();
	m_Layer_Editor->Initialise();
#endif

	PushLayer(m_Layer_Game);

#if WITH_EDITOR
	PushLayer(m_Layer_Editor);
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

		m_GameWorld->InitialiseWorld();

#if WITH_EDITOR
		m_GameWorld->StartEditor();
#else
		StartGame();
#endif // WITH_EDITOR



		m_Timer.Mark(); // Reset timer to avoid a long initial deltatime
	}

	Instrumentor::Get().EndSession();
	Instrumentor::Get().BeginSession("Game Running", "Saved\\Profiling-GameLoop.json");

	while (m_Running)
	{
		PROFILE_CURRENT_SCOPE("Game Loop");

		float DeltaTime = m_Timer.Mark();

		m_MainWindow->PreUpdate();
		m_MainWindow->OnUpdate();

		//If the window has sent a shutdown message return immediately
		if (!m_Running)
		{
			//Finish the profiling first
			Instrumentor::Get().EndSession();
			return;
		}

		Renderer::BeginScene();

		if (!m_GamePaused)
		{
			PROFILE_CURRENT_SCOPE("Game - Tick");

			//TODO: Check where to move the world since I'm using layers
			m_GameWorld->Tick(DeltaTime);
		}

		{
			PROFILE_CURRENT_SCOPE("Game - Render World");

			//TODO: Check where to move the world since I'm using layers
			m_GameWorld->Render();
		}



		{
			PROFILE_CURRENT_SCOPE("Game - Layer Updates");

			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate(DeltaTime);
			}
		}


		//= Debug Rendering =
		{
			PROFILE_CURRENT_SCOPE("Game - Draw Debug Physics");

			if (m_DrawCollision)
				m_GameWorld->GetPhysicsWorld()->debugDrawWorld();

			m_GameWorld->GetLineBatcher().DrawLines();
		}

		Renderer::EndScene();

		//= UI Rendering =
		{
			PROFILE_CURRENT_SCOPE("Game - ImGui Rendering");
			m_Layer_ImGui->Begin();
			for (Layer* layer : m_LayerStack)
			{
#if WITH_EDITOR
				layer->OnImGuiRender(m_RenderEditor);
#else
				layer->OnImGuiRender(false);
#endif
			}
			m_Layer_ImGui->End();
		}

		//= Post Update =

		m_MainWindow->PostUpdate();
	}

	Instrumentor::Get().EndSession();
}

void Application::OnEvent(Event& e)
{
	EventDispatcher Dispatcher(e);
	Dispatcher.Dispatch<WindowClosedEvent>(BIND_EVENT_FUNCTION(&Application::OnWindowClosed));
	Dispatcher.Dispatch<WindowResizedEvent>(BIND_EVENT_FUNCTION(&Application::OnWindowResized));

	for (auto iterator = m_LayerStack.end(); iterator != m_LayerStack.begin();)
	{
		(*--iterator)->OnEvent(e);
		if (e.m_Handled)
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
	m_Running = false;
	FLOW_ENGINE_LOG("Window Closed");
	return true;
}

bool Application::OnWindowResized(WindowResizedEvent& e)
{
	RenderCommand::Resize(e.GetWidth(), e.GetHeight());
	return false;
}

Application& Application::Get()
{
	return *sm_Application;
}

World* Application::GetWorld()
{
	return sm_Application->m_GameWorld;
}

void Application::Shutdown()
{
	Application::Get().m_Running = false;
}

void Application::SaveLevel()
{
	Application::GetWorld()->SaveLevel();
}

void Application::LoadLevel()
{
	Application::GetWorld()->LoadLevel();
}

void Application::NewLevel()
{
}

void Application::SavePlayState()
{
	Application::GetWorld()->SavePlayState();
}

void Application::LoadPlayState()
{
	Application::GetWorld()->LoadPlayState();
}

Window& Application::GetWindow()
{
	return *m_MainWindow;
}

bool Application::StartGame()
{
	Application& App = Application::Get();

	if (App.m_GameWorld->GetWorldState() == WorldState::InGame)
		return false;

	for (Layer* layer : App.m_LayerStack)
	{
		layer->BeginPlay();
	}
	App.m_GameWorld->StartGame();
	return true;
}

bool Application::PauseGame()
{
	Application& App = Application::Get();

	if (App.m_GameWorld->GetWorldState() != WorldState::InGame)
		return false;

	App.m_GameWorld->PauseGame();
	return true;
}

bool Application::StopGame()
{
	Application& App = Application::Get();

	if (App.m_GameWorld->GetWorldState() != WorldState::InGame)
		return false;

	App.m_GameWorld->StopGame();
	return true;
}