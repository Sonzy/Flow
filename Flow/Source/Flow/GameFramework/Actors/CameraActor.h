#pragma once
#include "Flow/GameFramework/ControlledObject.h"

namespace Flow
{
	class CameraComponent;
}

class FLOW_API CameraActor : public Flow::ControlledObject
{
public:
	CameraActor();
	CameraActor(const std::string& Name);

	void SetMainCamera() const;

protected:

	std::shared_ptr<Flow::CameraComponent> _CameraComponent;
};
