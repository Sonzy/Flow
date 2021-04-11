#pragma once

// Includes ///////////////////////////////////////////////////////////////////////////

#include "Framework/Types/ComPtr.h"
#include "Rendering/Core/Bindables/Bindable.h"

// Type Definitions ///////////////////////////////////////////////////////////////////

struct ID3D11VertexShader;
struct ID3D10Blob;
typedef ID3D10Blob ID3DBlob;

// Class Definition ///////////////////////////////////////////////////////////////////

namespace Bindables
{
	class VertexShader : public Bindables::Bindable
	{
	public:
		// Public Static Functions ////////////////////////////////////////////////////

		static VertexShader*					Resolve(const string& LocalPath);
		static HashString						GenerateID(const string& LocalPath);

		// Public Functions ///////////////////////////////////////////////////////////

												VertexShader(const string& LocalPath);
		void									Bind() override;
		ID3DBlob*								GetByteCode() const;
		HashString								GetID() override;

	private:

		// Private Variables ///////////////////////////////////////////////////////////

		string									m_shaderPath;
		ComPtr<ID3D11VertexShader>				m_vertexShader;
		ID3DBlob*								m_blob;
	};
}