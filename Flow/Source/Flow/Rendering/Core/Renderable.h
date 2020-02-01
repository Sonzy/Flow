#pragma once
#include "Bindable.h"
#include <DirectXMath.h>
#include "Bindables/IndexBuffer.h"

namespace Flow
{
	class FLOW_API Renderable
	{
	public:
		Renderable();
		Renderable(Renderable&&) = delete; //TODO: Why this will stop Vector Unique ptr errors, but deleting lvalue copy constructor doesnt.

		virtual void Update(float deltaTime) {};

		virtual DirectX::XMMATRIX GetTransformXM() const = 0;

		const IndexBuffer& GetIndexBuffer();
	protected:

		template<class T>
		T* GetBindable() noexcept
		{
			for (auto& pb : m_Binds)
			{
				if (auto pt = dynamic_cast<T*>(pb.get()))
					return pt;
			}
			return nullptr;
		}


		static void AddStaticBindable(std::unique_ptr<Bindable> Bind);

		void AddStaticIndexBuffer(std::unique_ptr<IndexBuffer> ibuffer);
		void SetIndexFromStatic();



	private:
		const std::vector<std::unique_ptr<Bindable>>& GetStaticBindables() const;

	private:
		const IndexBuffer* m_IndexBuffer = nullptr;
		std::vector<std::unique_ptr<Bindable>> m_Binds;

		static std::vector<std::unique_ptr<Bindable>> m_StaticBinds;
	};
}