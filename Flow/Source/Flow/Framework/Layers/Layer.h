#pragma once

//= Includes ===================================

#include "Flow/Core.h"
#include "Framework/Events/Event.h"

//= Forward Declarations =======================

class Controller;

//= Class Definition ===========================

class FLOW_API Layer
{
public:

	//= Public Functions ================

							Layer(const std::string& Name);
	virtual					~Layer();

	virtual void			BeginPlay();
	virtual void			OnAttach();
	virtual void			OnDetach();
	virtual void			OnUpdate(float DeltaTime);
	virtual void			OnEvent(Event& e);
	virtual void			OnImGuiRender(bool DrawEditor);

	const std::string&		GetName() const;
protected:

	//= Protected Variables ================

	std::string				m_Name;
};