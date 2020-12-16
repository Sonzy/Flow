#pragma once

//= Includes =======================================

#include <DirectXMath.h>
#include <memory>
#include "ShaderConstantBuffers.h"
#include "Rendering/Core/Renderable.h"

//= Forward Declarations ===========================

class RenderableComponent;

//= Class Definiton ================================

class TransformConstantBuffer : public Bindable
{
public:
	
	//= Public Functions ==================================================

								TransformConstantBuffer(Renderable* Parent, UINT VertexSlot = 3u);
	void						Bind() override;

protected:

	//= Protected Structs =================================================
	struct Transforms
	{
		DirectX::XMMATRIX		m_ModelView;
		DirectX::XMMATRIX		m_ModelViewProjection;
	};

protected:

	//= Protected Static Variables ========================================

	static std::unique_ptr<VertexConstantBuffer<Transforms>>		m_VertexConstBuffer;

protected:

	//= Protected Variables ===============================================

	std::string					m_Tag;
	Renderable*					m_ParentObject;
};