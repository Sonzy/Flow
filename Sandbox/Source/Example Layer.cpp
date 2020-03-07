#include "Example Layer.h"
#include "ThirdParty/ImGui/imgui.h"
#include "Flow\Rendering\Renderer.h"
#include "Flow\Rendering\RenderCommand.h"
#include "Flow\Assets\AssetSystem.h"
#include "Flow\Rendering\Core\Materials\Material.h"

#include "Flow\Application.h"
#include "Flow\GameFramework\World.h"

#include "Content/ExampleWorldObject.h"
#include "Flow\Rendering\Core\Camera\Camera.h"

ExampleLayer::ExampleLayer()
	: Layer("Example")
{
	for (int i = 0; i < 1; i++)
	{
		Actors.push_back(Flow::Application::GetWorld()->SpawnWorldObject<ExampleWorldObject>());
	}

	Light = std::make_shared<Flow::PointLight>(500.0f);
}

ExampleLayer::~ExampleLayer()
{
	Actors.clear();
}

void ExampleLayer::OnUpdate(float DeltaTime)
{
	int Count = 0;
	Flow::Renderer::BeginScene();

	Light->BindLight(Flow::RenderCommand::GetCamera().GetMatrix());

	for (auto& Actor : Actors)
	{
		Actor->Render();
	}

	Flow::Renderer::EndScene();

	Flow::RenderCommand::GetCamera().Tick(DeltaTime);
}

void ExampleLayer::OnImGuiRender()
{
	Flow::RenderCommand::GetCamera().RenderIMGUIWindow();

	Flow::AssetSystem::RenderDebugWindow(true);
	Light->RenderControlWindow();
}

void ExampleLayer::OnAttach()
{
	Layer::OnAttach();
}
