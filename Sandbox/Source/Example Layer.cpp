#include "Example Layer.h"
#include "ThirdParty/ImGui/imgui.h"
#include "Flow\Rendering\Renderer.h"
#include "Flow\Rendering\RenderCommand.h"
#include "Flow\Assets\AssetSystem.h"
#include "Flow\Rendering\Core\Materials\Material.h"

#include "Flow\Application.h"
#include "Flow\GameFramework\World.h"


ExampleLayer::ExampleLayer()
	: Layer("Example")
{
	TestWorldObject = Flow::Application::GetWorld()->SpawnWorldObject<ExampleWorldObject>();
}

void ExampleLayer::OnUpdate(float DeltaTime)
{
	Flow::RenderCommand::GetCamera().Tick(DeltaTime);

	//Light->Bind(Flow::RenderCommand::GetCamera().GetMatrix());

	int Count = 0;
	Flow::Renderer::BeginScene();
	for (auto Mesh : TestMesh)
	{
		//Count += Flow::Renderer::SubmitWithoutDraw(reinterpret_cast<Flow::Renderable*>(Mesh.get()));
		//Flow::Renderer::Submit(reinterpret_cast<Flow::Renderable*>(Mesh.get()));
		Flow::Renderer::Submit(Mesh.get());
		
	}

	//Flow::Renderer::Submit(Light->GetMesh());
	Flow::Renderer::EndScene();
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
