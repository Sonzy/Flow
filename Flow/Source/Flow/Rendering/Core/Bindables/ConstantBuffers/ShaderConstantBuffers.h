#pragma once
#include "ConstantBuffer.h"

namespace Flow
{
	//TODO: Might not want to use a constant buffer with the same signature multiple times. Might need to generate UID with a tag
	template<typename C>
	class PixelConstantBuffer : public ConstantBuffer<C>
	{
		//These allow you to access parent class stuff in a template child class by importing them
		//or could do this->GetContext(gfx)->PSSetConstantBuffers(0u, 1u, constantBuffer.GetAddressOf());
		using ConstantBuffer<C>::m_ConstantBuffer;
		using ConstantBuffer<C>::m_Slot;
		using ConstantBuffer<C>::Tag_;
	public:
		using ConstantBuffer<C>::ConstantBuffer;

		void Bind() override
		{
			RenderCommand::DX11GetContext()->PSSetConstantBuffers(m_Slot, 1u, m_ConstantBuffer.GetAddressOf());
		}


		//= Bindable Interface =

		static std::shared_ptr<Bindable> Resolve(const C& Consts, UINT Slot, const std::string& Tag)
		{
			return BindableCodex::Resolve<PixelConstantBuffer>(Consts, Slot, Tag);
		}

		static std::shared_ptr<Bindable> Resolve(const C& Consts, UINT Slot)
		{
			return BindableCodex::Resolve<PixelConstantBuffer>(Consts, Slot, "");
		}

		static std::string GenerateUID(const C& Consts, UINT Slot, const std::string& Tag)
		{
			using namespace std::string_literals;
			return typeid(PixelConstantBuffer).name()+ "#"s + Tag;
		}

		//std::string GetUID() const
		//{
		//	return GenerateUID();
		//}
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

		//= Bindable Interface =

		static std::shared_ptr<VertexConstantBuffer> Resolve()
		{
			BindableCodex::Resolve<VertexConstantBuffer>();
		}

		static std::string GenerateUID()
		{
			return typeid(VertexConstantBuffer).name();
		}

		std::string GetUID() const
		{
			return GenerateUID();
		}
	};
}