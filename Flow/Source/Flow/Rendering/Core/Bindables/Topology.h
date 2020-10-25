#pragma once

//= Includes =============================================

#include "Flow/Rendering/Core/Bindable.h"

//= Class Definitions ====================================

class Topology : public Bindable
{
public:
	
	//= Public Static Functions ===============================================


	//= Bindable Interface =

	static Bindable*		Resolve(D3D11_PRIMITIVE_TOPOLOGY type);
	static std::string						GenerateUID(D3D11_PRIMITIVE_TOPOLOGY type);
	std::string								GetUID() const override;
public:

	//= Public Functions ======================================================

											Topology(D3D11_PRIMITIVE_TOPOLOGY type);
	void									Bind() noexcept override;
	
protected:

	//= Private Variables =====================================================

	D3D11_PRIMITIVE_TOPOLOGY				m_Topology;
};