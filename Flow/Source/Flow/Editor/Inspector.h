#pragma once
#include "Flow/Events/MouseEvent.h"

namespace Flow
{
	class WorldObject;
	class World;
	class SelectionGizmo;

	class Inspector
	{
	public:
		Inspector(SelectionGizmo* Selector);

		void RenderInspector();
		void RenderHeirarchy();

		void SetCurrentWorld(World* WorldReference);

		bool OnMouseClicked(MouseButtonPressedEvent& e);

		void Update();

		SelectionGizmo* GetSelector();
	private:

		World* m_CurrentWorld;
		WorldObject* m_FocusedItem;
		SelectionGizmo* m_Selector;

		bool ObjChanged;
	};

}