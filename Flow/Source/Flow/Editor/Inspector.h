#pragma once

//= Includes ====================================

#include "Flow/Events/MouseEvent.h"

//= Forward Declarations ========================

class Actor;
class World;
class WorldComponent;
class KeyPressedEvent;

//= Class Declaration ===========================

/*	Inspector within the editor. Provides details of the current selected object as well as modifiable properties of the object.
	Also displays the world heirarchy in the editor. */
class Inspector
{
public:

	//= Public Functions =========================

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
	void							ClearFocus()							{ m_FocusedComponent = nullptr; }

private:

	//= Private Functions =========================

	void							RenderInspector();
	void							RenderHeirarchy();

private:

	//= Private Variables =========================

	World*							m_CurrentWorld;
	WorldComponent*					m_FocusedComponent;

	// Whether the focused item has changed this frame, so dont apply modifiers
	// to the new selected object from the previous one
	bool							m_HideTree;
};	