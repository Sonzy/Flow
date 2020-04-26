#pragma once
#include "Flow/Events/MouseEvent.h"

namespace Flow
{
	class WorldObject;
	class World;
	class SelectionGizmo;
	class WorldComponent;

	/*	Inspector within the editor. Provides details of the current selected object as well as modifiable properties of the object. 
		Also displays the world heirarchy in the editor. */
	class Inspector
	{
	public:
		Inspector(SelectionGizmo* Selector);

		void RenderInspector();
		void RenderHeirarchy();

		void SetCurrentWorld(World* WorldReference);

		bool OnMouseClicked(MouseButtonPressedEvent& e);

		void Update();

		static void UpdateSelectedComponent(WorldComponent* NewComp);
		static WorldComponent* GetSelectedComponent();

		[[nodiscard]] SelectionGizmo* GetSelector() const;

		void DrawSelectedComponentTransform();
	private:

		World* CurrentWorld_;
		WorldObject* FocusedItem_;
		WorldComponent* FocusedComponent_;
		SelectionGizmo* Selector_;

		// Whether the focused item has changed this frame, so dont apply modifiers
		// to the new selected object from the previous one
		bool FocusedItemChanged;
		bool FocusedComponentChanged;
		bool HideTree;
	};

}