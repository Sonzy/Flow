#pragma once

//= Includes ====================================

#include "ConstantBuffer.h"

//= Class Definitions ===========================

//TODO: Might not want to use a constant buffer with the same signature multiple times. Might need to generate UID with a tag
template<typename C>
class PixelConstantBuffer : public ConstantBuffer<C>
{
	//= Typedefs =========================================================

	//These allow you to access parent class stuff in a template child class by importing them
	//or could do this->GetContext(gfx)->PSSetConstantBuffers(0u, 1u, constantBuffer.GetAddressOf());
	using ConstantBuffer<C>::m_ConstantBuffer;
	using ConstantBuffer<C>::m_Slot;
	using ConstantBuffer<C>::m_Tag;
public:
	//= Public Typedefs ==================================================
	using ConstantBuffer<C>::ConstantBuffer;

public:

	//= Public Static Functions ===========================================

	//= Bindable Interface =

	static PixelConstantBuffer<C>* Resolve(const C& Consts, UINT Slot, const std::string& Tag)
	{
		return BindableCodex::Resolve<PixelConstantBuffer>(Consts, Slot, Tag);
	}

	static Bindable* Resolve(const C& Consts, UINT Slot)
	{
		return BindableCodex::Resolve<PixelConstantBuffer>(Consts, Slot, "");
	}

	static std::string GenerateUID(const C& Consts, UINT Slot, const std::string& Tag)
	{
		using namespace std::string_literals;
		return typeid(PixelConstantBuffer).name() + "#"s + Tag;
	}

public:

	//= Public Functions =================================================

	void Bind() override
	{
		RenderCommand::DX11GetContext()->PSSetConstantBuffers(m_Slot, 1u, m_ConstantBuffer.GetAddressOf());
	}
};

template<typename C>
class VertexConstantBuffer : public ConstantBuffer<C>
{
	//= Typedefs =========================================================

	//These allow you to access parent class stuff in a template child class
	using ConstantBuffer<C>::m_ConstantBuffer;
	using ConstantBuffer<C>::m_Slot;
	using ConstantBuffer<C>::m_Tag;
public:

	//= Public Typedefs ==================================================

	using ConstantBuffer<C>::ConstantBuffer;

public:

	//= Public Static Functions ===========================================

	//= Bindable Interface =

	static VertexConstantBuffer<C>* Resolve(const C& Consts, UINT Slot, const std::string& Tag)
	{
		return BindableCodex::Resolve<VertexConstantBuffer<C>>(Consts, Slot, Tag);
	}

	static Bindable* Resolve(const C& Consts, UINT Slot)
	{
		return BindableCodex::Resolve<VertexConstantBuffer<C>>(Consts, Slot, "");
	}

	static std::string GenerateUID(const C& Consts, UINT Slot, const std::string& Tag)
	{
		using namespace std::string_literals;
		return typeid(VertexConstantBuffer<C>).name() + "#"s + Tag;
	}

public:

	//= Public Functions =================================================

	void Bind() override
	{
		RenderCommand::DX11GetContext()->VSSetConstantBuffers(m_Slot, 1u, m_ConstantBuffer.GetAddressOf());
	}
};