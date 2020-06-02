#pragma once
#include "Flow/Events/MouseEvent.h"

class Actor;
class World;
class SelectionGizmo;
class WorldComponent;

/*	Inspector within the editor. Provides details of the current selected object as well as modifiable properties of the object.
	Also displays the world heirarchy in the editor. */
class Inspector
{
public:
	Inspector(SelectionGizmo* Selector);

	void BeginPlay();

	void Update();
	void Render();

	void SetCurrentWorld(World* WorldReference);
	bool OnMouseClicked(MouseButtonPressedEvent& e);

	bool OnMouseReleased(MouseButtonReleasedEvent& e);

	static void UpdateSelectedComponent(WorldComponent* NewComp);
	void DrawSelectedComponentTransform();

	static WorldComponent* GetSelectedComponent();
	[[nodiscard]] SelectionGizmo* GetSelector() const;

private:

	void RenderInspector();
	void RenderHeirarchy();

	World* _CurrentWorld;
	Actor* _FocusedItem;
	WorldComponent* _FocusedComponent;
	SelectionGizmo* _Selector;

	// Whether the focused item has changed this frame, so dont apply modifiers
	// to the new selected object from the previous one
	bool _FocusedItemChanged;
	bool _FocusedComponentChanged;
	bool _HideTree;
};