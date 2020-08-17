#pragma once
#include "Flow/Events/MouseEvent.h"

class Actor;
class World;
class WorldComponent;
class KeyPressedEvent;

/*	Inspector within the editor. Provides details of the current selected object as well as modifiable properties of the object.
	Also displays the world heirarchy in the editor. */
class Inspector
{
public:
									Inspector();

	void							BeginPlay();

	void							Update();
	void							Render();

	void							SetCurrentWorld(World* WorldReference);

	bool							OnMouseClicked(MouseButtonPressedEvent& e);
	bool							OnMouseReleased(MouseButtonReleasedEvent& e);
	bool							OnKeyPressed(KeyPressedEvent& e);

	static void						UpdateSelectedComponent(WorldComponent* NewComp);
	void							DrawSelectedComponentTransform();

	static WorldComponent*			GetSelectedComponent();

	void							SetFocus(WorldComponent* NewFocus)		{ m_FocusedComponent = NewFocus; };
	void							ClearFocus(WorldComponent* NewFocus)	{ m_FocusedComponent = nullptr; }

private:

	void							RenderInspector();
	void							RenderHeirarchy();

	World*							_CurrentWorld;
	//Actor* _FocusedItem;
	WorldComponent*					m_FocusedComponent;
	//SelectionGizmo* _Selector;

	// Whether the focused item has changed this frame, so dont apply modifiers
	// to the new selected object from the previous one
	//bool _FocusedItemChanged;
	//bool _FocusedComponentChanged;
	bool							_HideTree;
};	