#pragma once
#include "Flowpch.h"
#include "Flow/Core.h"
#include "Flow/Events/Event.h"

class Controller;

class FLOW_API Layer
{
public:
	Layer(const std::string& Name);
	virtual ~Layer();

	virtual void BeginPlay();
	virtual void OnAttach();
	virtual void OnDetach();
	virtual void OnUpdate(float DeltaTime);
	virtual void OnEvent(Event& e);

	virtual void OnImGuiRender(bool DrawEditor);



	const std::string& GetName() const;
protected:
	std::string _Name;


};