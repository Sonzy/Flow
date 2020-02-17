#include "Example Layer.h"
#include "ThirdParty/ImGui/imgui.h"
#include "Flow\Rendering\Renderer.h"
#include "Flow\Rendering\RenderCommand.h"
#include "Flow\Assets\AssetSystem.h"

ExampleLayer::ExampleLayer()
	: Layer("Example")
{
	std::shared_ptr<Flow::StaticMesh> NewMesh = std::make_shared<Flow::StaticMesh>("Hat_FancyMan");
	TestMesh.push_back(NewMesh);

	std::shared_ptr<Flow::StaticMesh> NewMesh2 = std::make_shared<Flow::StaticMesh>("Hat_FancyMan");
	NewMesh2->SetPosition(Vector(10.0f));
	TestMesh.push_back(NewMesh2);

	std::shared_ptr<Flow::StaticMesh> NewMesh3 = std::make_shared<Flow::StaticMesh>("Hat_FancyMan");
	NewMesh3->SetPosition(Vector(-10.0f));
	TestMesh.push_back(NewMesh3);


	Light = std::make_shared<Flow::PointLight>(1.0f);
}

void ExampleLayer::OnUpdate(float DeltaTime)
{
	Flow::RenderCommand::GetCamera().Tick(DeltaTime);

	Light->Bind(Flow::RenderCommand::GetCamera().GetMatrix());

	int Count = 0;
	Flow::Renderer::BeginScene();
	for (auto Mesh : TestMesh)
	{
		//Count += Flow::Renderer::SubmitWithoutDraw(reinterpret_cast<Flow::Renderable*>(Mesh.get()));
		Flow::Renderer::Submit(reinterpret_cast<Flow::Renderable*>(Mesh.get()));
	}

	Flow::Renderer::Submit(Light->GetMesh());
	//Flow::Renderer::Draw(Count);
	Flow::Renderer::EndScene();
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
