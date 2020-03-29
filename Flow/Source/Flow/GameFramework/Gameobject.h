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

		/* To be overriden, assumes Imgui::Begin and Imgui::End are being handled externally. Draws widgets for itself in here
		 * Typically DontUpdate is true if we have swapped objects, since it means that the references for object properties
		 * to update are incorrect, so we shouldnt write to any values */
		virtual void DrawDetailsWindow(bool bDontUpdate);

		const std::string& GetName();

	protected:

		std::string m_ObjectName;
	};
}