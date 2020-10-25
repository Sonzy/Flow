#pragma once

//= Includes ====================================

#include <vector>
#include "Flow/Rendering/Core/Bindable.h"

//= Class Definition ============================

class IndexBuffer : public Bindable
{
public:

	//= Public Static Functions =======================================

	static Bindable*							Resolve(const std::string& Tag, const std::vector<unsigned short>& Indices);

	/* Calls GenerateUID_Internal without passing the VBuffer */
	template<typename ...Ignore>
	static std::string GenerateUID(const std::string& Tag, Ignore&&... Ign)
	{
		return GenerateUID_Internal(Tag);

	}

	//= Public Functions ==============================================

												IndexBuffer(const std::string& Tag, const std::vector<unsigned short>& Indices);
	virtual void								Bind() override;
	UINT										GetCount() const;
	std::string									GetUID() const override;

	//= Public Variables ==============================================

	UINT										m_Count;
	Microsoft::WRL::ComPtr<ID3D11Buffer>		m_IndexBuffer;

private:

	//= Private Functions =============================================

	static std::string							GenerateUID_Internal(const std::string& Tag);

private:

	//= Private Variables =============================================

	std::string									m_Tag;

};