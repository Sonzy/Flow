#include "Example Layer.h"
#include "ThirdParty/ImGui/imgui.h"
#include "Flow\Rendering\Renderer.h"
#include "Flow\Rendering\RenderCommand.h"
#include "Flow\Assets\AssetSystem.h"
#include "Flow\Rendering\Core\Materials\Material.h"

#include "Flow\Application.h"
#include "Flow\GameFramework\World.h"

#include "Content\OpenCVTest.h"
#include "Content\MultiuseCube.h"

ExampleLayer::ExampleLayer()
	: Layer("Example")
{
	TestWorldObject = Flow::Application::GetWorld()->SpawnWorldObject<ExampleWorldObject>();
	Cube = Flow::Application::GetWorld()->SpawnWorldObject<MultiuseCube>();
	//CVTest = Flow::Application::GetWorld()->SpawnWorldObject<OpenCVTest>();
}

void ExampleLayer::OnUpdate(float DeltaTime)
{
	//Light->Bind(Flow::RenderCommand::GetCamera().GetMatrix());

	int Count = 0;
	Flow::Renderer::BeginScene();

	//TestWorldObject->Render();
	Cube->Render();
	//CVTest->Render();

	Flow::Renderer::EndScene();

	Flow::RenderCommand::GetCamera().Tick(DeltaTime);
}

void ExampleLayer::OnImGuiRender()
{
	Flow::RenderCommand::GetCamera().RenderIMGUIWindow();

	Flow::AssetSystem::RenderDebugWindow(true);

	//Light->RenderControlWindow();
}

void ExampleLayer::OnAttach()
{
	Layer::OnAttach();
}
