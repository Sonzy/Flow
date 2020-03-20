#include "Flowpch.h"
#include "Renderable.h"

namespace Flow
{
	void Renderable::AddBind(std::shared_ptr<Bindable> bind)
	{
		//If index buffer, only allow single bind.
		if (typeid(*bind) == typeid(IndexBuffer))
		{
			assert("Renderable::AddBind: Cannot bind multiple index buffers." && m_IndexBuffer == nullptr);
			m_IndexBuffer = static_cast<IndexBuffer*>(bind.get());
		}

		m_Binds.push_back(std::move(bind));
	}

	Renderable::Renderable()
		: m_Position(0.0f), m_Rotation(0.0f), m_Scale(1.0f)
	{
	}

	Renderable::~Renderable()
	{
		m_Binds.clear();
	}

	const IndexBuffer& Renderable::GetIndexBuffer()
	{
		return *m_IndexBuffer;
	}

	void Renderable::SetPosition(Vector Location)
	{
		m_Position = Location;
	}

	void Renderable::SetRotation(Rotator Rotation)
	{
		m_Rotation = Rotation;
	}

	void Renderable::SetScale(Vector Scale)
	{
		m_Scale = Scale;
	}

	void Renderable::BindAll()
	{
		for (auto& b : m_Binds)
		{
			b->Bind();
		}
	}
}


