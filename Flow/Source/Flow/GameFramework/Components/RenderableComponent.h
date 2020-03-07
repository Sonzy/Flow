#pragma once
#include "Flow\Core.h"
#include "Flow\GameFramework\Components\WorldComponent.h"
#include <memory>

namespace Flow
{
	class Bindable;
	class IndexBuffer;

	/* Base class for any components with rendering capabilities */
	class FLOW_API RenderableComponent : public WorldComponent
	{
	public:
		RenderableComponent();
		RenderableComponent(const std::string& Name);
	

		void AddBind(std::shared_ptr<Bindable> bind);
		void BindAll();

		const IndexBuffer& GetIndexBuffer() const;

		virtual DirectX::XMMATRIX GetTransformXM();

	protected:




		virtual void RefreshBinds();

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

		std::vector<std::shared_ptr<Bindable>> m_Binds;
		const IndexBuffer* m_IndexBuffer;
	};
}