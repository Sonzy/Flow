#pragma once

//= Includes ========================================

#include "Flow\Core.h"

//= Class Definitons ================================

class FLOW_API GameObject
{
public:

	//= Public Functions =====================

							GameObject();
							GameObject(const std::string& Name);
	virtual					~GameObject();

#if WITH_EDITOR
	virtual void			EditorBeginPlay() {};
	virtual void			OnViewportSelected() {};
	virtual void			OnViewportDeselected() {};
#endif

	virtual void			Tick(float DeltaTime);

	//To be overriden, assumes Imgui::Begin and Imgui::End are being handled externally. Draws widgets for itself in here
	//Typically DontUpdate is true if we have swapped objects, since it means that the references for object properties
	//to update are incorrect, so we shouldnt write to any values
	virtual void			DrawDetailsWindow(bool bDontUpdate);
	const std::string&		GetName() const;
	std::string&			GetWritableName();
	void					SetName(const std::string& NewName);

protected:

	//= Protected Variables ==================
	std::string				m_ObjectName;
};