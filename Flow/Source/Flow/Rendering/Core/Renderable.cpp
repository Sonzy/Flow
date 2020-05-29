#include "Flowpch.h"
#include "Renderable.h"

void Renderable::AddBind(std::shared_ptr<Bindable> bind)
{
	//If index buffer, only allow single bind.
	if (typeid(*bind) == typeid(IndexBuffer))
	{
		assert("Renderable::AddBind: Cannot bind multiple index buffers." && _IndexBuffer == nullptr);
		_IndexBuffer = static_cast<IndexBuffer*>(bind.get());
	}

	_Binds.push_back(std::move(bind));
}

Renderable::Renderable()
	: _Position(0.0f), _Rotation(0.0f), _Scale(1.0f)
{
}

Renderable::~Renderable()
{
	_Binds.clear();
}

const IndexBuffer& Renderable::GetIndexBuffer()
{
	return *_IndexBuffer;
}

void Renderable::SetPosition(Vector Location)
{
	_Position = Location;
}

void Renderable::SetRotation(Rotator Rotation)
{
	_Rotation = Rotation;
}

void Renderable::SetScale(Vector Scale)
{
	_Scale = Scale;
}

void Renderable::BindAll()
{
	for (auto& b : _Binds)
	{
		b->Bind();
	}
}


