#pragma once

//= Includes ////////////////////////////////////////////////////////////////

#include "Framework/Types/ComPtr.h"
#include "Rendering/Core/Bindables/Bindable.h"

// Forward Declarations /////////////////////////////////////////////////////

struct ID3D11BlendState;

//= Class Definition ////////////////////////////////////////////////////////

class BlendState : public Bindables::Bindable
{
public:

	// Public Static Functions //////////////////////////////////////////////

	static BlendState*						Resolve(bool enabled);
	static HashString					GenerateID(bool enabled);

public:

	// Public Functions /////////////////////////////////////////////////////

										BlendState(bool enabled);
	virtual void						Bind() override;

	//= Bindable Interface =

	virtual HashString					GetID() override;

private:

	// Private Variables /////////////////////////////////////////////////////

	ComPtr<ID3D11BlendState>			m_blendState;
	bool								m_enabled;
};