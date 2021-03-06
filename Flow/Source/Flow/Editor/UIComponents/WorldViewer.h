#pragma once

#if WITH_EDITOR

// Includes ////////////////////////////////

#include "UIComponent.h"

// Type Definitions ////////////////////////

class World;

// Class Defintion /////////////////////////

class WorldViewer : public UIComponent
{
public:
	WorldViewer();

	virtual void			Update() override;
	virtual void			Render() override;

private:

	// Private Variables //////////////////////

	World& m_World;
};

#endif // WITH_EDITOR