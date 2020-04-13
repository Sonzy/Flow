#pragma once
#include "Flow/Layers/Layer.h"

namespace Flow
{
	class WorldObject;
}

class MeshWorldObject;

class AGDLayer : public Flow::Layer
{
public:
	AGDLayer();
	~AGDLayer();

	/* Layer Interface */

	virtual void OnUpdate(float DeltaTime) override;
	virtual void OnImGuiRender() override;
	virtual void OnAttach() override;

private:

	/*=== Level Objects =============*/

	std::vector<std::shared_ptr<Flow::WorldObject>> WorldObjects_;

	//std::shared_ptr<MeshWorldObject> PlaneTest_;
	std::shared_ptr<Flow::WorldObject> Player_;
	std::shared_ptr<Flow::WorldObject> Base_;
};
