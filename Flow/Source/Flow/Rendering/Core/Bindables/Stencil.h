#pragma once

// Includes /////////////////////////////////////////////////////////////////////////

#include "Framework/Types/ComPtr.h"
#include "Rendering/Core/Bindables/Bindable.h"

// Type Definitions /////////////////////////////////////////////////////////////////

struct ID3D11DepthStencilState;

namespace Bindables
{
	// Class Definitions ////////////////////////////////////////////////////////////////

	class Stencil : public Bindables::Bindable
	{
	public:

		// Public Enums /////////////////////////////////////////////////////////////////

		enum class Mode
		{
			Off,
			Write,
			Mask,
			AlwaysOnTop,
			NoDepth
		};

	public:

		// Public Static Functions //////////////////////////////////////////////////////

		static Stencil*										Resolve(Stencil::Mode mode);
		static HashString									GenerateID(Stencil::Mode mode);
		static const char*									ModeToString(Stencil::Mode mode);

	public:

		// Public Functions ////////////////////////////////////////////////////////////

		Stencil(Stencil::Mode mode);
		virtual void										Bind() override;
		virtual HashString									GetID() override;


	private:

		// Private Variables //////////////////////////////////////////////////////////

		ComPtr<ID3D11DepthStencilState>						m_stencil;
		Stencil::Mode										m_mode;
	};
}