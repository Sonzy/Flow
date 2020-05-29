#pragma once
#include "Flow/Rendering/Core/Bindable.h"
#include "Flow/Rendering/Core/Vertex/VertexBuffer.h"
#include <wrl.h>

class BindableVertexBuffer : public Bindable
{
public:
	BindableVertexBuffer(const std::string& Tag, const VertexBuffer& Buffer);

	void Bind() override;


	//= Bindable Interface =

	static std::shared_ptr<Bindable> Resolve(const std::string& Tag, const VertexBuffer& Buffer);

	/* Calls GenerateUID_Internal without passing the VBuffer */
	template<typename ...Ignore>
	static std::string GenerateUID(const std::string& Tag, Ignore&&... Ign)
	{
		return GenerateUID_Internal(Tag);
	}

	std::string GetUID() const override;

protected:

	static std::string GenerateUID_Internal(const std::string& Tag);

	std::string _Tag; //Used to find the bindable in the codex
	UINT _Stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> _Buffer;
};