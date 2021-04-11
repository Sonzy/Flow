#pragma once

// Includes //////////////////////////////////////////////////////////////////////////

#include "ConstantBuffer.h"
#include "Rendering/Renderer.h"

// Class Definitions /////////////////////////////////////////////////////////////////

//TODO: Might not want to use a constant buffer with the same signature multiple times. Might need to generate UID with a tag
template<typename C>
class PixelConstantBuffer : public ConstantBuffer<C>
{
private:
	// Using Statements //////////////////////////////////////////////////////////////

	//These allow you to access parent class stuff in a template child class by importing them
	//or could do this->GetContext(gfx)->PSSetConstantBuffers(0u, 1u, constantBuffer.GetAddressOf());
	using Bindables::Bindable::m_id;
	using ConstantBuffer<C>::m_constantBuffer;
	using ConstantBuffer<C>::m_slot;
	using ConstantBuffer<C>::m_tag;
public:
	using ConstantBuffer<C>::ConstantBuffer;

public:

	// Public Static Functions ///////////////////////////////////////////////////////

	//= Bindable Interface =

	static PixelConstantBuffer<C>* Resolve(const C& Consts, UINT Slot, HashString Tag)
	{
		return Bindables::Codex::Resolve<PixelConstantBuffer>(Consts, Slot, Tag);
	}

	static PixelConstantBuffer<C>* Resolve(const C& Consts, UINT Slot)
	{
		return Bindables::Codex::Resolve<PixelConstantBuffer>(Consts, Slot, HashString());
	}

	static HashString GenerateID(const C& Consts, UINT Slot, HashString tag)
	{
		char buffer[64];
#if _DEBUG
		snprintf(buffer, 64, "PixelConstantBuffer-%s", tag.c_str());
#else
		snprintf(buffer, 64, "PixelConstantBuffer-%d", tag);
#endif
		return buffer;

	}

	virtual HashString GetID() override
	{
		if (m_id.IsNull())
		{
			C dummy{};
			m_id = GenerateID(dummy, m_slot, m_tag);
		}
		return m_id;
	}

public:

	// Public Functions /////////////////////////////////////////////////////////////

	void Bind() override
	{
		Renderer::GetContext()->PSSetConstantBuffers(m_slot, 1u, m_constantBuffer.GetAddressOf());
	}
};

template<typename C>
class VertexConstantBuffer : public ConstantBuffer<C>
{
	// Using Statements //////////////////////////////////////////////////////////////

	//These allow you to access parent class stuff in a template child class
	using Bindables::Bindable::m_id;
	using ConstantBuffer<C>::m_constantBuffer;
	using ConstantBuffer<C>::m_slot;
public:
	using ConstantBuffer<C>::ConstantBuffer;

public:

	// Public Static Functions ///////////////////////////////////////////////////////

	//= Bindable Interface =

	static VertexConstantBuffer<C>* Resolve()
	{
		Bindables::Codex::Resolve<VertexConstantBuffer>();
	}

	static HashString GenerateID()
	{
		return "VertexConstantBuffer";
	}

	virtual HashString GetID() override
	{
		if (m_id.IsNull())
		{
			m_id = GenerateID();
		}
		return m_id;
	}

public:

	// Public Functions ////////////////////////////////////////////////////////////

	void Bind() override
	{
		Renderer::GetContext()->VSSetConstantBuffers(m_slot, 1u, m_constantBuffer.GetAddressOf());
	}
};