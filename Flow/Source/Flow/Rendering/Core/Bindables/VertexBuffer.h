#pragma once

// Includes //////////////////////////////////////////////////////////////////////////////

#include "Typedefs.h"
#include "Framework/Types/ComPtr.h"
#include "Rendering/Core/Bindables/Bindable.h"
#include "Rendering/Core/Vertex/VertexBufferData.h"

namespace Bindables
{
	// Class Definition //////////////////////////////////////////////////////////////////

	class VertexBuffer : public Bindables::Bindable
	{
	public:

		// Public Static Functions ///////////////////////////////////////////////////////

		static VertexBuffer* Resolve(HashString tag, const ::VertexBuffer& buffer);

		/* Calls GenerateUID_Internal without passing the VBuffer */
		template<typename ...Ignore>
		static HashString GenerateID(HashString tag, Ignore&&... Ign)
		{
			return GenerateID_Internal(tag);
		}

		// Public Functions //////////////////////////////////////////////////////////////

		VertexBuffer(HashString tag, const ::VertexBuffer& Buffer);

		//= Bindable Interface =

		virtual void									Bind() override;
		HashString										GetID() override;
		const VertexLayout&								GetLayout() const;

	protected:

		//= Protected Functions ====================================================

		static HashString								GenerateID_Internal(HashString tag);

	protected:

		//= Protected Variables ====================================================

		HashString								m_tag;
		uint32									m_stride;
		ComPtr<ID3D11Buffer>					m_buffer;
		VertexLayout							m_layout;
	};

	inline const VertexLayout& VertexBuffer::GetLayout() const
	{
		return m_layout;
	}
}
