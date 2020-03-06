#pragma once
#include "Flow/Layers/Layer.h"
#include "Flow/Rendering/Core/Mesh/StaticMesh.h"
#include "Flow\Rendering\Core\Lights\PointLight.h"
#include <vector>

#include "ExampleWorldObject.h"

class OpenCVTest;
class MultiuseCube;

class ExampleLayer : public Flow::Layer
{
public:
	ExampleLayer();

	virtual void OnUpdate(float DeltaTime) override;
	virtual void OnImGuiRender() override;
	virtual void OnAttach() override;

private:
	std::vector<std::shared_ptr<Flow::StaticMesh>> TestMesh;
	std::shared_ptr<Flow::PointLight> Light;

	std::shared_ptr<ExampleWorldObject> TestWorldObject;
	std::shared_ptr<OpenCVTest> CVTest;
	std::shared_ptr<MultiuseCube> Cube;

	std::vector<std::shared_ptr<Flow::WorldObject>> Actors;
};