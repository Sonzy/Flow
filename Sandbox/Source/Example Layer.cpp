#include "Example Layer.h"
#include "ThirdParty/ImGui/imgui.h"
#include "Flow\Rendering\Renderer.h"
#include "Flow\Rendering\RenderCommand.h"

ExampleLayer::ExampleLayer()
	: Layer("Example")
{
	int CountCubed = 20;

	int offset = 10.0f;

		float Distance = 20.0f;

	for (int i = 0; i < CountCubed; i++)
	{
		for (int j = 0; j < CountCubed; j++)
		{
			for (int k = 0; k < CountCubed; k++)
			{
				std::shared_ptr<Flow::StaticMesh> NewMesh = std::make_shared<Flow::StaticMesh>("Flow\\Assets\\Models\\Box.obj");
				NewMesh->SetPosition(Vector((-Distance * (CountCubed / 2)) + (i * Distance), (-Distance * (CountCubed / 2)) + (j * Distance), (-Distance * (CountCubed / 2)) + (k * Distance)));
				NewMesh->SetScale(Vector(5.0f));
				TestMesh.push_back(NewMesh);
			}
		}
	}
}

void ExampleLayer::OnUpdate(float DeltaTime)
{
	Flow::RenderCommand::GetCamera().Tick(DeltaTime);

	int Count = 0;
	Flow::Renderer::BeginScene();
	for (auto Mesh : TestMesh)
	{
		Count+= Flow::Renderer::SubmitWithoutDraw(reinterpret_cast<Flow::Renderable*>(Mesh.get()));
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

	FLOW_ENGINE_LOG("FOOK");
}
