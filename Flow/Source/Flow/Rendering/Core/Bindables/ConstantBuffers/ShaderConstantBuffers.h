#pragma once
#include "ConstantBuffer.h"

namespace Flow
{
	template<typename C>
	class PixelConstantBuffer : public ConstantBuffer<C>
	{
		//These allow you to access parent class stuff in a template child class by importing them
		//or could do this->GetContext(gfx)->PSSetConstantBuffers(0u, 1u, constantBuffer.GetAddressOf());
		using ConstantBuffer<C>::constantBuffer;
		using ConstantBuffer<C>::slot;
	public:
		using ConstantBuffer<C>::ConstantBuffer;

		void Bind(Graphics& gfx) override
		{
			//TODO: GetContext(gfx)->PSSetConstantBuffers(slot, 1u, constantBuffer.GetAddressOf());
		}
	};

	template<typename C>
	class VertexConstantBuffer : public ConstantBuffer<C>
	{
		//These allow you to access parent class stuff in a template child class
		using ConstantBuffer<C>::constantBuffer;
		using ConstantBuffer<C>::slot;
	public:
		using ConstantBuffer<C>::ConstantBuffer;

		void Bind(Graphics& gfx) noexcept override
		{
			//TODO:GetContext(gfx)->VSSetConstantBuffers(slot, 1u, constantBuffer.GetAddressOf());
		}
	};
}