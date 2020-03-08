#pragma once
#include "Flow/Events/MouseEvent.h"

namespace Flow
{
	class WorldObject;
	class World;

	class Inspector
	{
	public:
		Inspector();

		void RenderInspector();
		void RenderHeirarchy();

		void SetCurrentWorld(World* WorldReference);

		bool OnMouseClicked(MouseButtonPressedEvent& e);
	private:

		World* m_CurrentWorld;
		WorldObject* m_FocusedItem;
	};

}