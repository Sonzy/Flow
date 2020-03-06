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
		virtual ~Renderable();

		virtual void Update(float deltaTime) {};

		virtual DirectX::XMMATRIX GetTransformXM() const = 0;

		const IndexBuffer& GetIndexBuffer();
		
		void SetPosition(Vector Location);
		void SetRotation(Rotator Rotation);
		void SetScale(Vector Scale);
		Vector GetPosition() { return m_Position; }
		Rotator GetRotation() { return m_Rotation; }
		Vector GetScale() { return m_Scale; }

		void BindAll();
		void AddBind(std::shared_ptr<Bindable> bind);

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

	protected:
		Vector m_Position;
		Vector m_Scale;
		Rotator m_Rotation;

	private:
		std::vector<std::shared_ptr<Bindable>> m_Binds;
		const IndexBuffer* m_IndexBuffer = nullptr;
	};
}