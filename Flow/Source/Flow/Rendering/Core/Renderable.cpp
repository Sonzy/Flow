#include "Flowpch.h"
#include "Renderable.h"

namespace Flow
{
	std::vector<std::unique_ptr<Bindable>> Renderable::staticBinds;

	void Renderable::AddStaticBindable(std::unique_ptr<Bindable> Bind)
	{
		assert("USE ADD INDEX BUFFER TO BIND INDEX BUFFER" && typeid(*Bind) != typeid(Bindable));
		staticBinds.push_back(std::move(Bind));
	}

	void Renderable::AddStaticIndexBuffer(std::unique_ptr<IndexBuffer> ibuffer)
	{
		assert("Attempting to add a 2nd index buffer" && indexBuffer == nullptr);
		indexBuffer = ibuffer.get();
		staticBinds.push_back(std::move(ibuffer));
	}

	void Renderable::SetIndexFromStatic()
	{
		assert("Attempting to add a 2nd index buffer" && indexBuffer == nullptr);
		for (const auto& b : staticBinds)
		{
			if (const auto p = dynamic_cast<IndexBuffer*>(b.get()))
			{
				indexBuffer = p;
				return;
			}
		}
		assert("Failed to find index buffer in static binds" && indexBuffer != nullptr);
	}

	const std::vector<std::unique_ptr<Bindable>>& Renderable::GetStaticBindables() const
	{
		return staticBinds;
	}
}


