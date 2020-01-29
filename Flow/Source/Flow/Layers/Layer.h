#pragma once
#include "Flowpch.h"
#include "Flow/Core.h"
#include "Flow/Events/Event.h"

namespace Flow
{
	class FLOW_API Layer
	{
	public:
		Layer(const std::string& Name);
		virtual ~Layer();

		virtual void OnAttach();
		virtual void OnDetach();
		virtual void OnUpdate();
		virtual void OnEvent(Event& e);

		virtual void OnImGuiRender();

		const std::string& GetName() const;
	protected:
		std::string Name;
	};
}