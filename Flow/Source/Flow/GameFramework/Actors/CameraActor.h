#pragma once

//= Includes =============================================

#include "Flow/GameFramework/Pawn.h"

//= Forward Declarations =================================

class CameraComponent;

//= Class Definition =====================================


class FLOW_API CameraActor : public Pawn
{
public:

	REGISTER_CLASS(CameraActor)

	//= Public Functions ========================

										CameraActor();
										CameraActor(const std::string& Name);

	virtual void						OnControlled(Controller* OwningController) override;
	virtual void						Tick(float DeltaTime) override;

	void								SetMainCamera() const;

protected:

	//= Protected Variables ======================

	CameraComponent*					m_CameraComponent;
	IntVector2							m_LastMousePosition;

	float								m_CameraMovementSpeed;
	float								m_CameraHorizontalRotationSpeed;
	float								m_CameraVerticalRotationSpeed;
};
