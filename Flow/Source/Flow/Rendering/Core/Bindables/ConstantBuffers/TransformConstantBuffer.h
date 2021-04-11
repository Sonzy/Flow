#pragma once

//= Includes =======================================

#include <DirectXMath.h>
#include <memory>
#include "ShaderConstantBuffers.h"
#include "Rendering/Core/Renderable.h"

//= Forward Declarations ===========================

class RenderableComponent;

//= Class Definiton ================================

class TransformConstantBuffer : public Bindables::Bindable
{
public:
	
	//= Public Functions ==================================================

								TransformConstantBuffer(Renderable* Parent);
	void						Bind() override;

protected:

	//= Protected Structs =================================================
	struct Transforms
	{
		DirectX::XMMATRIX		m_ModelView;
		DirectX::XMMATRIX		m_ModelViewProjection;

		DirectX::XMMATRIX		m_ModelViewProjectionNonTrans;
		DirectX::XMMATRIX		m_ModelViewNonTrans;
	};

protected:

	//= Protected Static Variables ========================================

	static std::unique_ptr<VertexConstantBuffer<Transforms>>		m_VertexConstBuffer;

protected:

	//= Protected Variables ===============================================

	std::string					m_Tag;
	Renderable*					m_ParentObject;
};