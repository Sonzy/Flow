#pragma once

// Includes ////////////////////////////////////////////////////////////

#include <vector>
#include "Framework/Types/ComPtr.h"
#include "Rendering/Core/Bindables/Bindable.h"
#include "Rendering/Core/Vertex/VertexLayout.h"

// Type Definitions ////////////////////////////////////////////////////

struct ID3D11InputLayout;

namespace Bindables
{
	// Class Definition ////////////////////////////////////////////////////

	class InputLayout : public Bindables::Bindable
	{
	public:

		// Public Static Functions /////////////////////////////////////////

		static InputLayout* Resolve(const VertexLayout& Layout, ID3DBlob* vertexShaderByteCode);
		static HashString						GenerateID(const VertexLayout& Layout, ID3DBlob* vertexShaderByteCode = nullptr);

		// Public Functions ////////////////////////////////////////////////////////////

		InputLayout(VertexLayout Layout, ID3DBlob* vertexShaderByteCode);
		virtual void							Bind() override;
		virtual HashString						GetID() override;

	private:

		// Private Variables ///////////////////////////////////////////////

		ComPtr<ID3D11InputLayout>						m_inputLayout;
		VertexLayout									m_vertexLayout;
	};
}