#pragma once

// Includes //////////////////////////////////////////////////////////

#include "Framework/Types/ComPtr.h"
#include "Rendering/Core/Bindables/Bindable.h"

// Type Definitions //////////////////////////////////////////////////

struct ID3D11SamplerState;

// Class Definition //////////////////////////////////////////////////

class Sampler : public Bindables::Bindable
{
public:

	// Public Static Functions ///////////////////////////////////////

	static Sampler*							Resolve();
	static HashString						GenerateID();
public:

	// Public Functions //////////////////////////////////////////////

											Sampler();
	virtual void							Bind() override;
	virtual HashString						GetID() override;
private:

	// Private Variables /////////////////////////////////////////////

	ComPtr<ID3D11SamplerState>				m_sampler;
};