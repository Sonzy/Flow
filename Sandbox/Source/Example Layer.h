#pragma once
#include "Flow/Layers/Layer.h"
#include "Flow/Rendering/Core/Mesh/StaticMesh.h"
#include "Flow\Rendering\Core\Lights\PointLight.h"
#include <vector>
#include <memory>

namespace Flow
{
	class WorldObject;
}

class ExampleLayer : public Flow::Layer
{
public:
	ExampleLayer();
	~ExampleLayer();

	virtual void OnUpdate(float DeltaTime) override;
	virtual void OnImGuiRender() override;
	virtual void OnAttach() override;

private:

	std::vector<std::shared_ptr<Flow::WorldObject>> Actors;
	std::shared_ptr<Flow::PointLight> Light;

	std::shared_ptr<Flow::StaticMesh> Mesh1;
	std::shared_ptr<Flow::StaticMesh> Mesh2;

	std::shared_ptr<Flow::StaticMesh> Floor;
	std::shared_ptr<Flow::StaticMesh> BackWall;
};