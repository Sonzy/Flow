#pragma once
#include "Bindable.h"
#include <DirectXMath.h>
#include "Bindables/IndexBuffer.h"
#include "Flow/Helper/Maths.h"

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
	Vector GetPosition() { return _Position; }
	Rotator GetRotation() { return _Rotation; }
	Vector GetScale() { return _Scale; }

	void BindAll();
	void AddBind(std::shared_ptr<Bindable> bind);

protected:

	template<class T>
	T* GetBindable() noexcept
	{
		for (auto& pb : _Binds)
		{
			if (auto pt = dynamic_cast<T*>(pb.get()))
				return pt;
		}
		return nullptr;
	}

protected:
	Vector _Position;
	Vector _Scale;
	Rotator _Rotation;

	std::vector<std::shared_ptr<Bindable>> _Binds;
	const IndexBuffer* _IndexBuffer = nullptr;
};