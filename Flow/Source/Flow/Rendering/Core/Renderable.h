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
		void SetRotation(Rotator Rotation);
		void SetScale(Vector Scale);
		Vector GetPosition() { return m_Position; }
		Rotator GetRotation() { return m_Rotation; }
		Vector GetScale() { return m_Scale; }

		void BindAll();
		void RefreshStaticBinds();

		void AddBind(std::shared_ptr<Bindable> bind);

		/* Replaces an existing bindable, if specified, will add the bindable anyway if it doesnt exist */
		// TODO: Validate type
		template<class T>
		bool ReplaceBindable(std::unique_ptr<T> NewBind, bool bInsertAnyway = false) noexcept
		{
			assert("MUST use AddIndexBuffer to bind index buffer" && typeid(*NewBind) != typeid(IndexBuffer));

			for (int i = 0; i < m_Binds.size(); i++)
			{
				if (auto pt = dynamic_cast<T*>(m_Binds[i].get()))
				{
					m_Binds[i] = std::move(NewBind);
					return true;
				}
			}

			m_Binds.push_back(std::move(NewBind));
			return false;
		}

		template<class T>
		bool ReplaceStaticBindable(std::unique_ptr<T> NewBind) noexcept
		{
			assert("USE ADD INDEX BUFFER TO BIND INDEX BUFFER" && typeid(*NewBind) != typeid(Bindable));

			for (int i = 0; i < m_StaticBinds.size(); i++)
			{
				if (auto pt = dynamic_cast<T*>(m_StaticBinds[i].get()))
				{
					m_StaticBinds[i] = std::move(NewBind);
					return true;
				}
			}

			return false;
		}

		template<class T>
		bool ReplaceOrAddStaticBindable(std::unique_ptr<T> NewBind) noexcept
		{
			assert("USE ADD INDEX BUFFER TO BIND INDEX BUFFER" && typeid(*NewBind) != typeid(Bindable));

			for (int i = 0; i < m_StaticBinds.size(); i++)
			{
				if (auto pt = dynamic_cast<T*>(m_StaticBinds[i].get()))
				{
					m_StaticBinds[i] = std::move(NewBind);
					return true;
				}
			}

			m_StaticBinds.push_back(std::move(NewBind));
			return false;
		}

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