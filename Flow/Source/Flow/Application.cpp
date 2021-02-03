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

	PROFILE_FUNCTION();

	sm_Application = this;
	m_EngineDirectory = std::filesystem::current_path().parent_path().append("Flow");
	m_GameDirectory = std::filesystem::current_path();

	//Create the main window
	{
		PROFILE_CURRENT_SCOPE("Window Creation");

		m_MainWindow = Window::Create(Window::Properties(m_ApplicationName, 1280u, 720u));
		m_MainWindow->SetEventCallback(BIND_EVENT_FUNCTION(&Application::OnEvent));
	}

	{
		PROFILE_CURRENT_SCOPE("ImGui Layer Initialisation");

		//Initialise the debug UI Layer
		m_Layer_ImGui = new ImGuiLayer();
		PushOverlay(m_Layer_ImGui);
	}

	{
		PROFILE_CURRENT_SCOPE("Factory Initialisation");

		//= Build class factory map before the application starts
		m_ClassFactory = new ClassFactory();
		m_ClassFactory->RegisterClassUIDs();
	}


	//= Models =

	{
		PROFILE_CURRENT_SCOPE("Asset System Initialisation");

		AssetSystem::Startup();
	}


	{
		PROFILE_CURRENT_SCOPE("Custom Materials");

		//= Materials =
		Mat_LitColor* litColorBlack = AssetSystem::CreateMaterial<Mat_LitColor>("Mat_LitColor_Black")->GetMaterial<Mat_LitColor>();
		litColorBlack->SetColor(Vector3(0.0f, 0.0f, 0.0f));
		Mat_LitColor* litColorGrey = AssetSystem::CreateMaterial<Mat_LitColor>("Mat_LitColor_Grey")->GetMaterial<Mat_LitColor>();
		litColorGrey->SetColor(Vector3(0.863f, 0.863f, 0.863f));
		Mat_LitColor* litColorSlate = AssetSystem::CreateMaterial<Mat_LitColor>("Mat_LitColor_Slate")->GetMaterial<Mat_LitColor>();
		litColorSlate->SetColor(Vector3(0.439f, 0.502f, 0.565f));
		Mat_LitColor* litColorBlue = AssetSystem::CreateMaterial<Mat_LitColor>("Mat_LitColor_Blue")->GetMaterial<Mat_LitColor>();
		litColorBlue->SetColor(Vector3(0.0f, 0.0f, 1.0f));
		Mat_LitColor* litColorRed = AssetSystem::CreateMaterial<Mat_LitColor>("Mat_LitColor_Red")->GetMaterial<Mat_LitColor>();
		litColorRed->SetColor(Vector3(1.0f, 0.0f, 0.0f));
		Mat_LitColor* litColorGreen = AssetSystem::CreateMaterial<Mat_LitColor>("Mat_LitColor_Green")->GetMaterial<Mat_LitColor>();
		litColorGreen->SetColor(Vector3(0.0f, 1.0f, 0.0f));

		Mat_LitColor* litColorDarkGreen = AssetSystem::CreateMaterial<Mat_LitColor>("Mat_LitColor_DarkGreen")->GetMaterial<Mat_LitColor>();
		litColorDarkGreen->SetColor(Vector3(0.0f, 0.35f, 0.0f));

		Mat_LitColor* litColorLightGreen = AssetSystem::CreateMaterial<Mat_LitColor>("Mat_LitColor_LightGreen")->GetMaterial<Mat_LitColor>();
		litColorLightGreen->SetColor(Vector3(0.412f, 1.0f, 0.459f));

		Mat_TexturedPhong* PropsMat = AssetSystem::CreateMaterial<Mat_TexturedPhong>("Mat_Wabble_Props")->GetMaterial<Mat_TexturedPhong>();
		PropsMat->SetTexture("Wabble_Props");
		PropsMat->SetPixelShader("TexturedPhong_PS");
		PropsMat->SetVertexShader("TexturedPhong_VS");

		Mat_TexturedPhong* WeaponsMat = AssetSystem::CreateMaterial<Mat_TexturedPhong>("Mat_Wabble_Weapons")->GetMaterial<Mat_TexturedPhong>();
		WeaponsMat->SetTexture("Wabble_Weapons");
		WeaponsMat->SetPixelShader("TexturedPhong_PS");
		WeaponsMat->SetVertexShader("TexturedPhong_VS");

		Mat_TexturedPhong* WoodMat = AssetSystem::CreateMaterial<Mat_TexturedPhong>("Mat_Wood")->GetMaterial<Mat_TexturedPhong>();
		WoodMat->SetTexture("Wabble_Wood");
		WoodMat->SetPixelShader("TexturedPhong_PS");
		WoodMat->SetVertexShader("TexturedPhong_VS");

		Mat_TexturedPhong* SandMat = AssetSystem::CreateMaterial<Mat_TexturedPhong>("Mat_Sand")->GetMaterial<Mat_TexturedPhong>();
		SandMat->SetTexture("Wabble_Sand");
		SandMat->SetPixelShader("TexturedPhong_PS");
		SandMat->SetVertexShader("TexturedPhong_VS");

		Mat_TexturedPhong* SkyMat = AssetSystem::CreateMaterial<Mat_TexturedPhong>("Mat_SkyCube")->GetMaterial<Mat_TexturedPhong>();
		SkyMat->SetTexture("YokohamaCubemap_4k");
		SkyMat->SetPixelShader("Texture_PS");
		SkyMat->SetVertexShader("Texture_VS");

		//= 2D Materials =

		ColorMaterial2D* flatColorWhite = AssetSystem::CreateMaterial<ColorMaterial2D>("Mat_FlatColour_White2D")->GetMaterial<ColorMaterial2D>();
		flatColorWhite->SetColour(Vector3(1.0f, 1.0f, 1.0f));
		ColorMaterial2D* flatColorGreen = AssetSystem::CreateMaterial<ColorMaterial2D>("Mat_FlatColour_Green2D")->GetMaterial<ColorMaterial2D>();
		flatColorGreen->SetColour(Vector3(0.0f, 1.0f, 0.0f));


		Mat_Texture2D* matTexture2D = AssetSystem::CreateMaterial<Mat_Texture2D>("Mat_Texture2D")->GetMaterial<Mat_Texture2D>();
		matTexture2D->SetTexture("Icon_Light");
		matTexture2D->SetPixelShader("Texture2D_PS");
	}

	{
		//Game Initialisation
		PROFILE_CURRENT_SCOPE("Game Initialisation");

		m_Layer_Game = new GameLayer();

		//Create the game world
		m_GameWorld = new World("Game World");
	}

#if WITH_EDITOR
	{
		//Game Initialisation
		PROFILE_CURRENT_SCOPE("Editor Initialisation");

		m_Layer_Editor = new Editor();
		m_Layer_Editor->Initialise();
	}
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