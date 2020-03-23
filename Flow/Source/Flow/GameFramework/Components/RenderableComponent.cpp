#include "Flowpch.h"
#include "RenderableComponent.h"
#include "Flow\Rendering\Core\Bindables\IndexBuffer.h"

namespace Flow
{
	RenderableComponent::RenderableComponent()
		: WorldComponent("Unnamed RenderableComponent"), m_IndexBuffer(nullptr)
	{
	}

	RenderableComponent::RenderableComponent(const std::string& Name)
		: WorldComponent(Name), m_IndexBuffer(nullptr)
	{
	}

	void RenderableComponent::BindAll()
	{
		for (auto& b : m_Binds)
		{
			b->Bind();
		}
	}

	const IndexBuffer& RenderableComponent::GetIndexBuffer() const
	{
		return *m_IndexBuffer;
	}

	DirectX::XMMATRIX RenderableComponent::GetTransformXM()
	{
		Transform WorldTransform = GetWorldTransform();
		Rotator RadianRotation = Rotator::AsRadians(WorldTransform.m_Rotation);
		return 	DirectX::XMMatrixScaling(WorldTransform.m_Scale.X, WorldTransform.m_Scale.Y, WorldTransform.m_Scale.Z) *
			DirectX::XMMatrixRotationRollPitchYaw(RadianRotation.Pitch, RadianRotation.Yaw, RadianRotation.Roll) * //Rotate around box centre
			DirectX::XMMatrixTranslation(WorldTransform.m_Location.X, WorldTransform.m_Location.Y, WorldTransform.m_Location.Z);

	}

	void RenderableComponent::AddBind(std::shared_ptr<Bindable> bind)
	{
		//If index buffer, only allow single bind.
		if (typeid(*bind) == typeid(IndexBuffer))
		{
			assert("Renderable::AddBind: Cannot bind multiple index buffers." && m_IndexBuffer == nullptr);
			m_IndexBuffer = static_cast<IndexBuffer*>(bind.get());
		}

		m_Binds.push_back(std::move(bind));
	}
	void RenderableComponent::RefreshBinds()
	{
	}
}