#include "Example Layer.h"
#include "ThirdParty/ImGui/imgui.h"
#include "Flow\Rendering\Renderer.h"
#include "Flow\Rendering\RenderCommand.h"

ExampleLayer::ExampleLayer()
	: Layer("Example")
{
	std::shared_ptr<Flow::StaticMesh> NewMesh = std::make_shared<Flow::StaticMesh>("Flow\\Assets\\Models\\Hat_FancyMan.obj");
	TestMesh.push_back(NewMesh);

	std::shared_ptr<Flow::StaticMesh> NewMesh2 = std::make_shared<Flow::StaticMesh>("Flow\\Assets\\Models\\Hat_FancyMan.obj");
	NewMesh2->SetPosition(Vector(10.0f));
	TestMesh.push_back(NewMesh2);

	std::shared_ptr<Flow::StaticMesh> NewMesh3 = std::make_shared<Flow::StaticMesh>("Flow\\Assets\\Models\\Hat_FancyMan.obj");
	NewMesh3->SetPosition(Vector(-10.0f));
	TestMesh.push_back(NewMesh3);
}

void ExampleLayer::OnUpdate(float DeltaTime)
{
	Flow::RenderCommand::GetCamera().Tick(DeltaTime);

	int Count = 0;
	Flow::Renderer::BeginScene();
	for (auto Mesh : TestMesh)
	{
		//Count += Flow::Renderer::SubmitWithoutDraw(reinterpret_cast<Flow::Renderable*>(Mesh.get()));
		Flow::Renderer::Submit(reinterpret_cast<Flow::Renderable*>(Mesh.get()));
	}
	Flow::Renderer::Draw(Count);
	Flow::Renderer::EndScene();
}

void ExampleLayer::OnImGuiRender()
{
	Flow::RenderCommand::GetCamera().RenderIMGUIWindow();
}

void ExampleLayer::OnAttach()
{
	Layer::OnAttach();
}
