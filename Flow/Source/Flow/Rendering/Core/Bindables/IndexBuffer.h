#pragma once

// Includes ///////////////////////////////////////////////////////////////////////////

#include "Framework/Types/Array.h"
#include "Framework/Types/ComPtr.h"
#include "Rendering/Core/Bindables/Bindable.h"

struct ID3D11Buffer;

// Namespace //////////////////////////////////////////////////////////////////////////

namespace Bindables
{
	class IndexBuffer : public Bindables::Bindable
	{
	public:

		// Public Static Functions ///////////////////////////////////////////////////

		static IndexBuffer* Resolve(HashString id, const Array<uint16>& Indices);

		template<typename ...IgnoredTypes>
		static HashString GenerateID(HashString id, IgnoredTypes&&... Ign)
		{
			return GenerateID_Internal(id);
		}

	public:

		// Public Functions ///////////////////////////////////////////////////////////

													IndexBuffer(HashString id, const Array<uint16>& Indices);

		//= Bindable Interface =

		virtual void								Bind() override;
		HashString									GetID() override;

		uint32										GetCount() const;

	private:

		// Private Static Functions ////////////////////////////////////////////////////

		static HashString							GenerateID_Internal(HashString tag);

	private:

		// Private Variables ///////////////////////////////////////////////////////////

		uint32										m_count;
		ComPtr<ID3D11Buffer>						m_buffer;
		HashString									m_tag;
	};

	// Inline Functions ////////////////////////////////////////////////////////////////

	inline uint32 IndexBuffer::GetCount() const
	{
		return m_count;
	}
}
