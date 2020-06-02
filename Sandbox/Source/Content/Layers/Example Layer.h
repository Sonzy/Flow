#pragma once
#include "Flow/Layers/Layer.h"
#include "Flow/Rendering/Core/Mesh/StaticMesh.h"
#include "Flow\Rendering\Core\Lights\PointLight.h"
#include <vector>
#include <memory>

class Actor;
class MeshWorldObject;
class CameraActor;

class ExampleLayer : public Layer
{
public:
	ExampleLayer();
	~ExampleLayer();

	virtual void BeginPlay() override;
	virtual void OnUpdate(float DeltaTime) override;
	virtual void OnImGuiRender(bool DrawEditor) override;
	virtual void OnAttach() override;

private:

	std::vector<std::shared_ptr<Actor>> Actors;
	std::shared_ptr<PointLight> Light;

	std::shared_ptr<MeshWorldObject> TestCube;

	std::shared_ptr<Actor> Example;

	std::shared_ptr<MeshWorldObject> WallObj;
	std::shared_ptr<MeshWorldObject> Base;
	std::shared_ptr<MeshWorldObject> Base2;

	std::shared_ptr<MeshWorldObject> Crate1_;
	std::shared_ptr<MeshWorldObject> Crate2_;
	std::shared_ptr<MeshWorldObject> Crate3_;
	std::shared_ptr<MeshWorldObject> Crate4_;
	std::shared_ptr<MeshWorldObject> Crate5_;

	std::shared_ptr<MeshWorldObject> Chair1_;
	std::shared_ptr<MeshWorldObject> Chair2_;
	std::shared_ptr<MeshWorldObject> Chair3_;
	std::shared_ptr<MeshWorldObject> Chair4_;

	std::shared_ptr<MeshWorldObject> Table1_;
	std::shared_ptr<MeshWorldObject> Table2_;

	std::shared_ptr<MeshWorldObject> Revolver_;
	std::shared_ptr<MeshWorldObject> Shotgun_;
	std::shared_ptr<MeshWorldObject> Blunderbuss_;
	std::shared_ptr<MeshWorldObject> Rifle_;

	std::shared_ptr<CameraActor> _CameraActor;
};