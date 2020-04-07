#include "Flowpch.h"
#include "Renderable.h"

namespace Flow
{
	void Renderable::AddBind(std::shared_ptr<Bindable> bind)
	{
		//If index buffer, only allow single bind.
		if (typeid(*bind) == typeid(IndexBuffer))
		{
			assert("Renderable::AddBind: Cannot bind multiple index buffers." && IndexBuffer_ == nullptr);
			IndexBuffer_ = static_cast<IndexBuffer*>(bind.get());
		}

		Binds_.push_back(std::move(bind));
	}

	Renderable::Renderable()
		: Position_(0.0f), Rotation_(0.0f), Scale_(1.0f)
	{
	}

	Renderable::~Renderable()
	{
		Binds_.clear();
	}

	const IndexBuffer& Renderable::GetIndexBuffer()
	{
		return *IndexBuffer_;
	}

	void Renderable::SetPosition(Vector Location)
	{
		Position_ = Location;
	}

	void Renderable::SetRotation(Rotator Rotation)
	{
		Rotation_ = Rotation;
	}

	void Renderable::SetScale(Vector Scale)
	{
		Scale_ = Scale;
	}

	void Renderable::BindAll()
	{
		for (auto& b : Binds_)
		{
			b->Bind();
		}
	}
}


