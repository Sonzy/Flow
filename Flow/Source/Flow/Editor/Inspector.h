#pragma once
#include "Flow/Events/MouseEvent.h"

class WorldObject;
class World;
class SelectionGizmo;

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

	[[nodiscard]] SelectionGizmo* GetSelector() const;
private:

	World* _CurrentWorld;
	WorldObject* _FocusedItem;
	SelectionGizmo* _Selector;

	// Whether the focused item has changed this frame, so dont apply modifiers
	// to the new selected object from the previous one
	bool _FocusedItemChanged;
};