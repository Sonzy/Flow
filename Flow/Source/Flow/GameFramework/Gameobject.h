#pragma once
#include "Flow\Core.h"

namespace Flow
{
	class FLOW_API GameObject
	{
	public:
		GameObject(); 
		GameObject(const std::string& Name);
		virtual ~GameObject();

		virtual void Tick(float DeltaTime);

		/* To be overriden, assumes Imgui::Begin and Imgui::End are being handled externally. Draws widgets for itself in here */
		virtual void DrawDetailsWindow();

		const std::string& GetName();

	protected:

		std::string m_ObjectName;
	};
}