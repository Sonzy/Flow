#include "Flowpch.h"
#include "Application.h"

//= Misc Includes =
#include "Logging/Log.h"


//= Input Includes =
#include "Events/ApplicationEvent.h"
#include "Input/Input.h"
#include "ThirdParty\ImGui\imgui.h"

//= Editor Includes =
#if WITH_EDITOR
#include "Editor/UIComponents/Inspector.h"
#include "Editor/Editor.h"
#endif

//= Asset Includes =
#include "Assets\AssetSystem.h"
#include "Assets\Materials\Mat_FlatColour.h"
#include "Assets\Materials\ColorMaterial2D.h"
#include "Assets\Materials\Mat_TexturedPhong.h"
#include "Assets\Materials\Mat_LitColor.h"
#include "Assets\Materials\Mat_Texture2D.h"
#include "Assets\Meshes\MeshAsset.h"

//= Helper Inclues =
#include "Utils\Profiling.h"
#include "GameFramework/Other/ClassFactory.h"

//= Game Framework includes =
#include "GameFramework\World.h"
#include "Layers/GameLayer.h"

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

#if WITH_EDITOR
	, m_RenderEditor(true)
	, m_Layer_Editor(nullptr)
#endif

{
}

void Application::InitialiseApplication()
{
	Instrumentor::Get().BeginSession("Application Startup", "Saved/Profiling-Startup.json");

	FLOW_ENGINE_LOG("================ Initialising Engine ======================");

	PROFILE_FUNCTION();

	sm_Application = this;
	m_EngineDirectory = std::filesystem::current_path().parent_path().append("Flow");
	m_GameDirectory = std::filesystem::current_path();

	//Create the main window
	m_MainWindow = Window::Create(Window::Properties(m_ApplicationName, 1280u, 720u));
	m_MainWindow->SetEventCallback(BIND_EVENT_FUNCTION(&Application::OnEvent));

	//Initialise the debug UI Layer
	m_Layer_ImGui = new ImGuiLayer();
	PushOverlay(m_Layer_ImGui);

	//= Build class factory map before the application starts
	m_ClassFactory = new ClassFactory();
	m_ClassFactory->RegisterClassUIDs();

	//= Models =

	AssetSystem::Startup();

	//= Materials =
	AssetSystem::CreateMaterial<Mat_LitColor>("Mat_LitColor_Black");
	static_cast<Mat_LitColor*>(AssetSystem::GetAsset<MaterialAsset>("Mat_LitColor_Black")->GetMaterial())->SetColor(Vector3(0.0f, 0.0f, 0.0f));
	AssetSystem::CreateMaterial<Mat_LitColor>("Mat_LitColor_Grey");
	static_cast<Mat_LitColor*>(AssetSystem::GetAsset<MaterialAsset>("Mat_LitColor_Grey")->GetMaterial())->SetColor(Vector3(0.863f, 0.863f, 0.863f));
	AssetSystem::CreateMaterial<Mat_LitColor>("Mat_LitColor_Slate");
	static_cast<Mat_LitColor*>(AssetSystem::GetAsset<MaterialAsset>("Mat_LitColor_Slate")->GetMaterial())->SetColor(Vector3(0.439f, 0.502f, 0.565f));
	AssetSystem::CreateMaterial<Mat_LitColor>("Mat_LitColor_Blue");
	static_cast<Mat_LitColor*>(AssetSystem::GetAsset<MaterialAsset>("Mat_LitColor_Blue")->GetMaterial())->SetColor(Vector3(0.0f, 0.0f, 1.0f));
	AssetSystem::CreateMaterial<Mat_LitColor>("Mat_LitColor_Red");
	static_cast<Mat_LitColor*>(AssetSystem::GetAsset<MaterialAsset>("Mat_LitColor_Red")->GetMaterial())->SetColor(Vector3(1.0f, 0.0f, 0.0f));
	AssetSystem::CreateMaterial<Mat_LitColor>("Mat_LitColor_Green");
	static_cast<Mat_LitColor*>(AssetSystem::GetAsset<MaterialAsset>("Mat_LitColor_Green")->GetMaterial())->SetColor(Vector3(0.0f, 1.0f, 0.0f));

	AssetSystem::CreateMaterial<Mat_TexturedPhong>("Mat_Wabble_Props");
	Mat_TexturedPhong* PropsMat = static_cast<Mat_TexturedPhong*>(AssetSystem::GetAsset<MaterialAsset>("Mat_Wabble_Props")->GetMaterial());
	PropsMat->SetTexture("Wabble_Props");
	PropsMat->SetPixelShader("TexturedPhong_PS");
	PropsMat->SetVertexShader("TexturedPhong_VS");

	AssetSystem::CreateMaterial<Mat_TexturedPhong>("Mat_Wabble_Weapons");
	Mat_TexturedPhong* WeaponsMat = static_cast<Mat_TexturedPhong*>(AssetSystem::GetAsset<MaterialAsset>("Mat_Wabble_Weapons")->GetMaterial());
	WeaponsMat->SetTexture("Wabble_Weapons");
	WeaponsMat->SetPixelShader("TexturedPhong_PS");
	WeaponsMat->SetVertexShader("TexturedPhong_VS");

	AssetSystem::CreateMaterial<Mat_TexturedPhong>("Mat_Wood");
	Mat_TexturedPhong* WoodMat = static_cast<Mat_TexturedPhong*>(AssetSystem::GetAsset<MaterialAsset>("Mat_Wood")->GetMaterial());
	WoodMat->SetTexture("Wabble_Wood");
	WoodMat->SetPixelShader("TexturedPhong_PS");
	WoodMat->SetVertexShader("TexturedPhong_VS");

	AssetSystem::CreateMaterial<Mat_TexturedPhong>("Mat_Sand");
	Mat_TexturedPhong* SandMat = static_cast<Mat_TexturedPhong*>(AssetSystem::GetAsset<MaterialAsset>("Mat_Sand")->GetMaterial());
	SandMat->SetTexture("Wabble_Sand");
	SandMat->SetPixelShader("TexturedPhong_PS");
	SandMat->SetVertexShader("TexturedPhong_VS");

	AssetSystem::CreateMaterial<Mat_TexturedPhong>("Mat_SkyCube");
	Mat_TexturedPhong* SkyMat = static_cast<Mat_TexturedPhong*>(AssetSystem::GetAsset<MaterialAsset>("Mat_SkyCube")->GetMaterial());
	//SkyMat->SetTexture("SkyCube_Test");
	SkyMat->SetTexture("YokohamaCubemap_4k");
	SkyMat->SetPixelShader("Texture_PS");
	SkyMat->SetVertexShader("Texture_VS");

	//= 2D Materials =

	AssetSystem::CreateMaterial<ColorMaterial2D>("Mat_FlatColour_White2D");
	static_cast<ColorMaterial2D*>(AssetSystem::GetAsset<MaterialAsset>("Mat_FlatColour_White2D")->GetMaterial())->SetColour(Vector3(1.0f, 1.0f, 1.0f));
	AssetSystem::CreateMaterial<ColorMaterial2D>("Mat_FlatColour_Green2D");
	static_cast<ColorMaterial2D*>(AssetSystem::GetAsset<MaterialAsset>("Mat_FlatColour_Green2D")->GetMaterial())->SetColour(Vector3(0.0f, 1.0f, 0.0f));


	AssetSystem::CreateMaterial<Mat_Texture2D>("Mat_Texture2D");


	FLOW_ENGINE_LOG("================ Initialising Game ======================");

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

	FLOW_ENGINE_LOG("================ Initialisation Complete ==================");
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
		m_Layer_Editor->InitialiseEditor();
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
	Dispatcher.Dispatch<WindowMinimizedEvent>(BIND_EVENT_FUNCTION(&Application::OnWindowMinimized));

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
	if (RenderCommand::IsMinimized())
	{
		RenderCommand::SetMinimized(false);
	}

	RenderCommand::Resize(e.GetWidth(), e.GetHeight());
	return false;
}

bool Application::OnWindowMinimized(WindowMinimizedEvent& e)
{
	RenderCommand::SetMinimized(true);
	return false;
}

bool Application::OnWindowRestored(WindowRestoredEvent& e)
{
	RenderCommand::SetMinimized(false);
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