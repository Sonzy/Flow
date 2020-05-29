#include "CameraActor.h"
#include "Flow/Rendering/RenderCommand.h"

CameraActor::CameraActor()
	: CameraActor("Camera Actor")
{
}

CameraActor::CameraActor(const std::string& Name)
{
	_CameraComponent = CreateComponent<Flow::CameraComponent>("Camera");
}

void CameraActor::SetMainCamera() const
{
	RenderCOmm
}
