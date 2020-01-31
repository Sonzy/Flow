#pragma once
#include "Bindable.h"
#include <DirectXMath.h>

#include "Bindables/BindableVertexBuffer.h"
#include "Bindables/IndexBuffer.h"
#include "Bindables/Sampler.h"
#include "Bindables/InputLayout.h"
#include "Bindables/Shaders/VertexShader.h"
#include "Bindables/Shaders/PixelShader.h"
#include "Bindables/ConstantBuffers/ConstantBuffer.h"
#include "Bindables/ConstantBuffers/ShaderConstantBuffers.h"
#include "Bindables/ConstantBuffers/TransformConstantBuffer.h"

namespace Flow
{
	class Renderable
	{
	public:

		void Draw() const;
		virtual void Update(float deltaTime) {};

		virtual DirectX::XMMATRIX GetTransformXM() const = 0;


	protected:

		template<class T>
		T* GetBindable() noexcept
		{
			for (auto& pb : binds)
			{
				if (auto pt = dynamic_cast<T*>(pb.get()))
					return pt;
			}
			return nullptr;
		}


		static void AddStaticBindable(std::unique_ptr<Bindable> Bind);

		void AddStaticIndexBuffer(std::unique_ptr<IndexBuffer> ibuffer);
		void SetIndexFromStatic();

	private:
		const std::vector<std::unique_ptr<Bindable>>& GetStaticBindables() const;

	private:
		const IndexBuffer* indexBuffer = nullptr;
		std::vector<std::unique_ptr<Bindable>> binds;

		static std::vector<std::unique_ptr<Bindable>> staticBinds;
	};
}