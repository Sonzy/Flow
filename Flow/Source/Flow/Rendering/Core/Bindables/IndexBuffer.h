#pragma once
#include <vector>
#include "Flow/Rendering/Core/Bindable.h"

namespace Flow
{
	class FLOW_API IndexBuffer : public Bindable
	{
	public:
		IndexBuffer(const std::string& Tag, const std::vector<unsigned short>& Indices);
		~IndexBuffer();

		virtual void Bind() override;

		UINT GetCount() const;


		//= Bindable Interface =

		static std::shared_ptr<Bindable> Resolve(const std::string& Tag, const std::vector<unsigned short>& Indices);

		/* Calls GenerateUID_Internal without passing the VBuffer */
		template<typename ...Ignore>
		static std::string GenerateUID(const std::string& Tag, Ignore&&... Ign)
		{
			return GenerateUID_Internal(Tag);
		}

		std::string GetUID() const override;

	protected:

		static std::string GenerateUID_Internal(const std::string& Tag);

		std::string Tag_;
	public:

		UINT Count_;
		Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer_;
	};
}