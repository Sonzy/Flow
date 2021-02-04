#pragma once

// Includes =====================================================

#include "Flow/Rendering/Core/Bindable.h"
#include "Flow/Rendering/Core/Vertex/VertexBuffer.h"
#include <wrl.h>

//= Class Definition ============================================

class BindableVertexBuffer : public Bindable
{
public:

	//= Public Static Functions ================================================

	static Bindable*		Resolve(const std::string& Tag, const VertexBuffer& Buffer);

	/* Calls GenerateUID_Internal without passing the VBuffer */
	template<typename ...Ignore>
	static std::string GenerateUID(const std::string& Tag, Ignore&&... Ign)
	{
		return GenerateUID_Internal(Tag);
	}

	//= Public Functions ========================================================

											BindableVertexBuffer(const std::string& Tag, const VertexBuffer& Buffer);
	void									Bind() override;
	std::string								GetUID() const override;
	const VertexLayout&						GetLayout() const;

protected:

	//= Protected Functions ====================================================

	static std::string						GenerateUID_Internal(const std::string& Tag);

protected:

	//= Protected Variables ====================================================

	std::string								m_Tag; //Used to find the bindable in the codex
	UINT									m_Stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_Buffer;

#if WITH_EDITOR
	VertexLayout							m_layout;
	std::vector<D3D11_INPUT_ELEMENT_DESC>	m_d3dlayout;
#endif
};	