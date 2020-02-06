#pragma once
#include "Flow/Layers/Layer.h"
#include "Flow/Rendering/Core/Mesh/StaticMesh.h"
#include <vector>

class ExampleLayer : public Flow::Layer
{
public:
	ExampleLayer();

	virtual void OnUpdate(float DeltaTime) override;
	virtual void OnImGuiRender() override;
	virtual void OnAttach() override;

private:
	std::vector<std::shared_ptr<Flow::StaticMesh>> TestMesh;
};