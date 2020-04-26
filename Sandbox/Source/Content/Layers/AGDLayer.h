#pragma once
#include "Flow/Layers/Layer.h"
#include "Flow\Helper\Timer.h"

namespace Flow
{
	class WorldObject;
	class Sprite;
	class BasicCamera;
	class PointLight;
}

class MeshWorldObject;
class OpenCVTesting;
class WorldGenerator;
class PlayerPlane;

class AGDLayer : public Flow::Layer
{
public:
	AGDLayer();
	~AGDLayer();

	/* Layer Interface */

	virtual void OnUpdate(float DeltaTime) override;
	virtual void OnImGuiRender() override;
	virtual void OnAttach() override;
	virtual void PostBeginPlay() override;

	static OpenCVTesting* GetCVTester();

private:

	/*=== Level Objects =============*/

	std::vector<std::shared_ptr<Flow::WorldObject>> WorldObjects_;

	//std::shared_ptr<MeshWorldObject> PlaneTest_;
	std::shared_ptr<PlayerPlane> Player_;
	std::shared_ptr<Flow::WorldObject> Base_;
	std::shared_ptr<Flow::WorldObject> Map_;
	std::shared_ptr<Flow::WorldObject> TestCube_;
	std::shared_ptr<Flow::WorldObject> Wall_;

	std::shared_ptr<Flow::PointLight> Light_;

	WorldGenerator* Generator_;

	Flow::BasicCamera* Camera_;

	std::shared_ptr<Flow::Sprite> Sprite_;

	std::vector<std::shared_ptr<MeshWorldObject>> GeneratedWorld;

	static OpenCVTesting* CVTesting_;

	bool UseOpenCV = true;
	bool OpenCVStarted = false;

	Flow::Timer TrackingTimer;

	float CubeColour_[3] = { 0.2f, 0.2f, 0.2f };
	bool UseRotatedLightVector = false;

	//TESTING

	bool DrawLast = true;
};
