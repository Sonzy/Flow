#include "Example Layer.h"
#include "ThirdParty/ImGui/imgui.h"

ExampleLayer::ExampleLayer()
	: Layer("Example")
{
}

void ExampleLayer::OnImGuiRender()
{
	ImGui::Begin("Test Window");
	ImGui::Text("Whoops");
	ImGui::End();
}

void ExampleLayer::OnAttach()
{
	Layer::OnAttach();

	FLOW_ENGINE_LOG("FOOK");
}
