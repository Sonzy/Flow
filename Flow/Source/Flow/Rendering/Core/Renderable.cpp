#include "Flowpch.h"
#include "Renderable.h"

namespace Flow
{
	std::vector<std::unique_ptr<Bindable>> Renderable::m_StaticBinds;

	void Renderable::AddStaticBindable(std::unique_ptr<Bindable> Bind)
	{
		assert("USE ADD INDEX BUFFER TO BIND INDEX BUFFER" && typeid(*Bind) != typeid(Bindable));
		m_StaticBinds.push_back(std::move(Bind));
	}
	
	void Renderable::AddStaticIndexBuffer(std::unique_ptr<IndexBuffer> ibuffer)
	{
		assert("Attempting to add a 2nd index buffer" && m_IndexBuffer == nullptr);
		m_IndexBuffer = ibuffer.get();
		m_StaticBinds.push_back(std::move(ibuffer));
	}
	
	void Renderable::SetIndexFromStatic()
	{
		assert("Attempting to add a 2nd index buffer" && m_IndexBuffer == nullptr);
		for (const auto& b : m_StaticBinds)
		{
			if (const auto p = dynamic_cast<IndexBuffer*>(b.get()))
			{
				m_IndexBuffer = p;
				return;
			}
		}
		assert("Failed to find index buffer in static binds" && m_IndexBuffer != nullptr);
	}

	Renderable::Renderable()
	{
	}

	const IndexBuffer& Renderable::GetIndexBuffer()
	{
		return *m_IndexBuffer;
	}

	void Renderable::BindAll()
	{
		for (auto& b : m_Binds)
		{
			b->Bind();
		}

		for (auto& b : m_StaticBinds)
		{
			b->Bind();
		}
	}

	const std::vector<std::unique_ptr<Bindable>>& Renderable::GetStaticBindables() const
	{
		return m_StaticBinds;
	}
}


