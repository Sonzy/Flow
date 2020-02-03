#pragma once
#include "ConstantBuffer.h"

namespace Flow
{
	template<typename C>
	class PixelConstantBuffer : public ConstantBuffer<C>
	{
		//These allow you to access parent class stuff in a template child class by importing them
		//or could do this->GetContext(gfx)->PSSetConstantBuffers(0u, 1u, constantBuffer.GetAddressOf());
		using ConstantBuffer<C>::m_ConstantBuffer;
		using ConstantBuffer<C>::m_Slot;
	public:
		using ConstantBuffer<C>::ConstantBuffer;

		void Bind() override
		{
			RenderCommand::DX11GetContext()->PSSetConstantBuffers(m_Slot, 1u, m_ConstantBuffer.GetAddressOf());
		}
	};

	template<typename C>
	class VertexConstantBuffer : public ConstantBuffer<C>
	{
		//These allow you to access parent class stuff in a template child class
		using ConstantBuffer<C>::m_ConstantBuffer;
		using ConstantBuffer<C>::m_Slot;
	public:
		using ConstantBuffer<C>::ConstantBuffer;

		void Bind() override
		{
			RenderCommand::DX11GetContext()->VSSetConstantBuffers(m_Slot, 1u, m_ConstantBuffer.GetAddressOf());
		}
	};
}