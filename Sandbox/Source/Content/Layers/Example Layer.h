#pragma once
#include "Flow/Layers/Layer.h"
#include "Flow/Rendering/Core/Mesh/StaticMesh.h"
#include "Flow\Rendering\Core\Lights\PointLight.h"
#include <vector>
#include <memory>

class Actor;
class StaticMeshActor;
class CameraActor;
class SkyboxActor;
class PointLightActor;

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

	std::shared_ptr<StaticMeshActor> TestCube;

	std::shared_ptr<Actor> Example;

	std::shared_ptr<StaticMeshActor> WallObj;
	std::shared_ptr<StaticMeshActor> Base;
	std::shared_ptr<StaticMeshActor> Base2;

	std::shared_ptr<StaticMeshActor> Crate1_;
	std::shared_ptr<StaticMeshActor> Crate2_;
	std::shared_ptr<StaticMeshActor> Crate3_;
	std::shared_ptr<StaticMeshActor> Crate4_;
	std::shared_ptr<StaticMeshActor> Crate5_;

	std::shared_ptr<StaticMeshActor> Chair1_;
	std::shared_ptr<StaticMeshActor> Chair2_;
	std::shared_ptr<StaticMeshActor> Chair3_;
	std::shared_ptr<StaticMeshActor> Chair4_;

	std::shared_ptr<StaticMeshActor> Table1_;
	std::shared_ptr<StaticMeshActor> Table2_;

	std::shared_ptr<StaticMeshActor> Revolver_;
	std::shared_ptr<StaticMeshActor> Shotgun_;
	std::shared_ptr<StaticMeshActor> Blunderbuss_;
	std::shared_ptr<StaticMeshActor> Rifle_;

	std::shared_ptr<CameraActor> _CameraActor;
	std::shared_ptr<SkyboxActor> _Skybox;
	std::shared_ptr<PointLightActor> _PointLightActor;
};