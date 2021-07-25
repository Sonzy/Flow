#include "pch.h"
#include "Controller.h"
#include "Flow\GameFramework\Pawn.h"
#include "Flow/GameFramework/Components/WorldComponent.h"

Controller::Controller()
	: Controller("Controller")
{
}

Controller::Controller(const std::string& Name)
	: Actor(Name)
	, m_ControlledObject(nullptr)
	, m_Camera(nullptr)
{
	m_Root = CreateComponent<WorldComponent>("Root Component");
	m_Root->SetOwned(true);
	m_rootComponent = m_Root;
}

void Controller::ControlObject(Pawn* Obj)
{
	if (m_ControlledObject)
	{
		RemoveControl(m_ControlledObject);
	}

	Obj->Control(this);
}

void Controller::RemoveControl(Pawn* Obj)
{
	CHECK_RETURN(!m_ControlledObject, "Controller::RemoveControl: Tried to remove control when no object is being controlled");

	m_ControlledObject->RemoveControl();
}

void Controller::SetCamera(CameraBase* NewCamera)
{
	CHECK_RETURN(!NewCamera, "Controller::SetCamera: Tried to remove control when no object is being controlled");
	m_Camera = NewCamera;
}

CameraBase* Controller::GetCamera() const
{
	return m_Camera;
}

bool Controller::IsLocalController() const
{
	return m_LocalController;
}

unsigned int Controller::GetControllerIndex() const
{
	return m_ControllerIndex;
}