#pragma once

// Includes ///////////////////////////////////////////////////////////////////////////

#include "Rendering/Core/Bindables/Bindable.h"
#include <d3d11.h>

// Namespace //////////////////////////////////////////////////////////////////////////

namespace Bindables
{
	// Class Definition ///////////////////////////////////////////////////////////////

	class Topology : public Bindables::Bindable
	{
	public:

		// Public Static Functions ////////////////////////////////////////////////////

		//= Bindable Interface =

		static Topology*						Resolve(D3D11_PRIMITIVE_TOPOLOGY type);
		static HashString						GenerateID(D3D11_PRIMITIVE_TOPOLOGY type);

	public:

		// Public Functions ///////////////////////////////////////////////////////////

												Topology(D3D11_PRIMITIVE_TOPOLOGY type);
		virtual void							Bind() override;

		//= Bindable Interface =

		virtual HashString						GetID();

	private:

		// Private Variables ///////////////////////////////////////////////////////////

		D3D11_PRIMITIVE_TOPOLOGY				m_topology;
	};
}
