#pragma once

// Includes ////////////////////////////////////////////////////

#include "UIComponent.h"
#include "Events/MouseEvent.h"

// Forward Declarations ////////////////////////////////////////

class Actor;
class World;
class WorldComponent;
class KeyPressedEvent;

// Class Declaration ///////////////////////////////////////////

//Inspector within the editor. Provides details of the current selected object as well as modifiable properties of the object.
//Also displays the world heirarchy in the editor.
class Inspector : public UIComponent
{
public:

	// Public Functions ////////////////////////////////////////

									Inspector();

	//= UI Component Interface =

	virtual void					Update() override;
	virtual void					Render() override;

	//= Events =

	bool							OnKeyPressed(KeyPressedEvent& e);

	//= Selection =

	WorldComponent*					GetSelectedComponent();

private:

	//= Private Functions =========================

	void							RenderInspector();
	void							RenderHeirarchy();

	void							DrawSelectedComponentTransform();

private:

	//= Private Variables =========================

	WorldComponent*					m_SelectedComponent;
	World&							m_CurrentWorld;
	Actor*							m_RenameActor;

	char							m_RenameBuffer[128];

	// Whether the focused item has changed this frame, so dont apply modifiers
	// to the new selected object from the previous one
	bool							m_HideTree;
	bool							m_HideWholeComponentTree; // Whether to render the component tree at all
};	