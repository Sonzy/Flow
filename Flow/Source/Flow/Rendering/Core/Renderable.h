#pragma once
#include "Bindable.h"
#include <DirectXMath.h>
#include "Bindables/IndexBuffer.h"
#include "Flow/Helper/Maths.h"

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
		
		void SetPosition(Vector Location);
		Vector GetPosition() { return m_Position; }
		Rotator GetRotation() { return m_Rotation; }

		void BindAll();
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

		void AddBind(std::unique_ptr<Bindable> bind);
		static void AddStaticBindable(std::unique_ptr<Bindable> Bind);

		void AddStaticIndexBuffer(std::unique_ptr<IndexBuffer> ibuffer);
		void SetIndexFromStatic();

		bool StaticInitialised() { return !m_StaticBinds.empty(); }



	private:
		const std::vector<std::unique_ptr<Bindable>>& GetStaticBindables() const;

	private:
		const IndexBuffer* m_IndexBuffer = nullptr;
		std::vector<std::unique_ptr<Bindable>> m_Binds;

		static std::vector<std::unique_ptr<Bindable>> m_StaticBinds;

	protected:
		Vector m_Position;
		Rotator m_Rotation;
	};
}