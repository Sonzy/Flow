// Pch //////////////////////////////////////////////////////////////////////////

#include "pch.h"

// Main Include /////////////////////////////////////////////////////////////////

#include "Application.h"

// Includes /////////////////////////////////////////////////////////////////////

#include "Assets/AssetSystem.h"
#include "Assets/Materials/Mat_FlatColour.h"
#include "Assets/Materials/ColorMaterial2D.h"
#include "Assets/Materials/Mat_TexturedPhong.h"
#include "Assets/Materials/Mat_LitColor.h"
#include "Assets/Materials/Mat_Texture2D.h"
#include "Assets/Meshes/MeshAsset.h"
#include "Framework/Events/ApplicationEvent.h"
#include "Framework/Input/Input.h"
#include "Framework/Layers/GameLayer.h"
#include "Framework/Logging/Log.h"
#include "Framework/Utils/Profiling.h"
#include "GameFramework/Other/ClassFactory.h"
#include "GameFramework/World.h"
#include "ThirdParty/ImGui/imgui.h"

// Editor Only //
#if WITH_EDITOR
#include "Editor/Editor.h"
#include "Editor/UIComponents/Inspector.h"
#endif

// Macros ///////////////////////////////////////////////////////////////////////

#define BIND_EVENT_FUNCTION(FunctionPtr) std::bind(FunctionPtr, this, std::placeholders::_1)

// Static Definitions ///////////////////////////////////////////////////////////

Application* Application::sm_Application = nullptr;

// Function Declarations ////////////////////////////////////////////////////////

Application::Application(const std::string& AppName)
	: m_name(AppName)
	, m_window(nullptr)
	, m_gameLayer(nullptr)
	, m_running(true)
	, m_drawCollision(false)
	, m_world(nullptr)
	, m_classFactory(nullptr)
#if WITH_EDITOR
	, m_renderEditor(true)
	, m_editorLayer(nullptr)
	, m_imGuiLayer(nullptr)
#endif
{
}

void Application::InitialiseApplication()
{
	Instrumentor::Get().BeginSession("Application Startup", "Saved/Profiling-Startup.json");

	PROFILE_FUNCTION();

	sm_Application = this;
	m_engineDirectory = std::filesystem::current_path().parent_path().append("Flow");
	m_gameDirectory = std::filesystem::current_path();

	{
		PROFILE_CURRENT_SCOPE("Window Creation");

		m_window = Window::Create(Window::Properties(m_name, 1280u, 720u));
		m_window->SetEventCallback(BIND_EVENT_FUNCTION(&Application::OnEvent));
	}

#if WITH_EDITOR
	{
		PROFILE_CURRENT_SCOPE("ImGui Layer Initialisation");

		m_imGuiLayer = new ImGuiLayer();
		PushOverlay(m_imGuiLayer);
	}
#endif //WITH_EDITOR

	{
		PROFILE_CURRENT_SCOPE("Factory Initialisation");

		m_classFactory = new ClassFactory();
		m_classFactory->RegisterClassUIDs();
	}

	{
		PROFILE_CURRENT_SCOPE("Asset System Initialisation");

		AssetSystem::Startup();
	}

	{
		PROFILE_CURRENT_SCOPE("Custom Materials");

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

		Mat_TexturedPhong* SkyBlueMat = AssetSystem::CreateMaterial<Mat_TexturedPhong>("Mat_Skybox_Blue")->GetMaterial<Mat_TexturedPhong>();
		SkyBlueMat->SetTexture("Blue");
		SkyBlueMat->SetPixelShader("Texture_PS");
		SkyBlueMat->SetVertexShader("Texture_VS");

		Mat_FlatColour* flatColorGreen = AssetSystem::CreateMaterial<Mat_FlatColour>("Mat_FlatColor_Green")->GetMaterial<Mat_FlatColour>();
		flatColorGreen->SetColour(Vector3(0.0f, 1.0f, 0.0f));

		ColorMaterial2D* flatColorWhite2D = AssetSystem::CreateMaterial<ColorMaterial2D>("Mat_FlatColour_White2D")->GetMaterial<ColorMaterial2D>();
		flatColorWhite2D->SetColour(Vector3(1.0f, 1.0f, 1.0f));
		ColorMaterial2D* flatColorGreen2D = AssetSystem::CreateMaterial<ColorMaterial2D>("Mat_FlatColour_Green2D")->GetMaterial<ColorMaterial2D>();
		flatColorGreen2D->SetColour(Vector3(0.0f, 1.0f, 0.0f));

		Mat_Texture2D* matTexture2D = AssetSystem::CreateMaterial<Mat_Texture2D>("Mat_Texture2D")->GetMaterial<Mat_Texture2D>();
		matTexture2D->SetTexture("Icon_Light");
		matTexture2D->SetPixelShader("Texture2D_PS");

		Mat_TexturedPhong* SpriteTest = AssetSystem::CreateMaterial<Mat_TexturedPhong>("Mat_SpriteTest")->GetMaterial<Mat_TexturedPhong>();
		SpriteTest->SetTexture("Blue");
		SpriteTest->SetPixelShader("Texture_PS");
		SpriteTest->SetVertexShader("Texture_VS");
	}

	{
		//Game Initialisation
		PROFILE_CURRENT_SCOPE("Game Initialisation");

		m_gameLayer = new GameLayer();
		m_world = new World("Game World");
	}

#if WITH_EDITOR
	{
		PROFILE_CURRENT_SCOPE("Editor Initialisation");

		m_editorLayer = new Editor();
		m_editorLayer->Initialise();
	}
#endif

	PushLayer(m_gameLayer);
#if WITH_EDITOR
	PushLayer(m_editorLayer);
#endif
}

Application::~Application()
{
	FLOW_ENGINE_LOG("Shutting Down Engine");

	Renderer::Shutdown();
	AssetSystem::Shutdown();
}

void Application::Run()
{
	{
		PROFILE_CURRENT_SCOPE("Run Initialisation");

		m_world->InitialiseWorld();

#if WITH_EDITOR
		m_editorLayer->InitialiseEditor();
		m_world->StartEditor();
#else
		StartGame();
#endif // WITH_EDITOR

		// Reset timer to avoid a long initial deltatime
		m_timer.Mark(); 
	}

	Instrumentor::Get().EndSession();
	Instrumentor::Get().BeginSession("Game Running", "Saved\\Profiling-GameLoop.json");

	while (m_running)
	{
		PROFILE_CURRENT_SCOPE("Game Loop");

		float DeltaTime = m_timer.Mark();

		m_window->PreUpdate();
		m_window->OnUpdate();

		//If the window has sent a shutdown message return immediately
		if (m_running == false)
		{
			//Finish the profiling first
			Instrumentor::Get().EndSession();
			return;
		}

		Renderer::BeginScene();

		if (m_world->IsGamePaused() == true)
		{
			PROFILE_CURRENT_SCOPE("Game - Tick");

			//TODO: Check where to move the world since I'm using layers
			m_world->Tick(DeltaTime);
		}

		{
			PROFILE_CURRENT_SCOPE("Game - Render World");

			//TODO: Check where to move the world since I'm using layers
			m_world->Render();
		}

		{
			PROFILE_CURRENT_SCOPE("Game - Layer Updates");

			for (Layer* layer : m_layerStack)
			{
				layer->OnUpdate(DeltaTime);
			}
		}

		{
			PROFILE_CURRENT_SCOPE("Game - Draw Debug Physics");

			if (m_drawCollision == true)
			{
				m_world->GetPhysicsWorld()->debugDrawWorld();
			}

			m_world->GetLineBatcher().DrawLines();
		}

		Renderer::EndScene();

#if WITH_EDITOR
		{
			PROFILE_CURRENT_SCOPE("Game - ImGui Rendering");

			m_imGuiLayer->Begin();

			for (Layer* layer : m_layerStack)
			{

				layer->OnImGuiRender(m_renderEditor);

			}

			m_imGuiLayer->End();
		}
#endif
		m_window->PostUpdate();
	}

	Instrumentor::Get().EndSession();
}

void Application::OnEvent(Event& e)
{
	EventDispatcher Dispatcher(e);
	Dispatcher.Dispatch<WindowClosedEvent>(BIND_EVENT_FUNCTION(&Application::OnWindowClosed));
	Dispatcher.Dispatch<WindowResizedEvent>(BIND_EVENT_FUNCTION(&Application::OnWindowResized));
	Dispatcher.Dispatch<WindowMinimizedEvent>(BIND_EVENT_FUNCTION(&Application::OnWindowMinimized));

	for (auto iterator = m_layerStack.end(); iterator != m_layerStack.begin();)
	{
		(*--iterator)->OnEvent(e);

		if (e.m_Handled == true)
		{
			break;
		}
	}
}

void Application::PushLayer(Layer* layer)
{
	m_layerStack.PushLayer(layer);
	layer->OnAttach();
}

void Application::PushOverlay(Layer* layer)
{
	m_layerStack.PushOverlay(layer);
	layer->OnAttach();
}

bool Application::OnWindowClosed(WindowClosedEvent& e)
{
	FLOW_ENGINE_LOG("Window Closed");

	m_running = false;
	return true;
}

bool Application::OnWindowResized(WindowResizedEvent& e)
{
	Renderer::SetMinimized(false);
	Renderer::Resize(e.GetWidth(), e.GetHeight());

	return false;
}

bool Application::OnWindowMinimized(WindowMinimizedEvent& e)
{
	Renderer::SetMinimized(true);
	return false;
}

bool Application::OnWindowRestored(WindowRestoredEvent& e)
{
	Renderer::SetMinimized(false);
	return false;
}

Application& Application::Get()
{
	return *sm_Application;
}

World* Application::GetWorld()
{
	return sm_Application->m_world;
}

void Application::Shutdown()
{
	Application::Get().m_running = false;
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

Window& Application::GetWindow()
{
	return *m_window;
}

bool Application::StartGame()
{
	Application& application = Application::Get();

	World::Get().LoadLevel();

	if (application.m_world->GetWorldState() == WorldState::InGame)
	{
		return false;
	}

	for (Layer* layer : application.m_layerStack)
	{
		layer->BeginPlay();
	}

	application.m_world->StartGame();
	return true;
}