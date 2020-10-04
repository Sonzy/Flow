#pragma once

//= Includes ========================================

#include <DirectXMath.h>
#include "Core.h"
#include "Rendering/Core/RenderableBase.h"

//= Forward Declarations ============================

class Material;

//= Class Definition ================================

class FLOW_API StaticMesh : public RenderableBase
{
public:

	//= Public Functions ============================================

								StaticMesh(const std::string& LocalPath);
	void						InitialiseStaticMesh(const std::string& LocalPath, Material* MaterialOverride);
	virtual DirectX::XMMATRIX	GetTransformXM() const override;

protected:

	//= Protected Variables =========================================

	Vector3						m_Position;
	Rotator						m_Rotation;
	Vector3						m_Scale;
};