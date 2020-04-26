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
		void AddBatchedBind(std::shared_ptr<Bindable> bind);

		void BindAll();
		void BindBatchedBinds();
		void BindNonBatched();

		const IndexBuffer& GetIndexBuffer() const;

		virtual DirectX::XMMATRIX GetTransformXM();

		virtual void DrawDetailsWindow(bool DontUpdate) override;
	protected:

		virtual void RefreshBinds();

		template<class T>
		T* GetBindable() noexcept
		{
			for (auto& pb : Binds_)
			{
				if (auto pt = dynamic_cast<T*>(pb.get()))
					return pt;
			}
			return nullptr;
		}

		std::vector<std::shared_ptr<Bindable>> Binds_;
		std::vector<std::shared_ptr<Bindable>> BatchedBinds_; //binds that only need 1 instance
		const IndexBuffer* IndexBuffer_;
	};
}