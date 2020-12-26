#pragma once

//= Includes ==============================================================

#include "Flow/GameFramework/Components/RenderableComponent.h"
#include "Flow\Rendering\Core\Bindables\ConstantBuffers\ShaderConstantBuffers.h"

//= Class Definitions =====================================================


class PointLightComponent : public RenderableComponent
{
public:

	REGISTER_CLASS(PointLightComponent)

	//= Public Functions ======================================

										PointLightComponent();
										PointLightComponent(const std::string& ComponentName);

	virtual void						Render() override;
	virtual void						DrawComponentDetailsWindow() override;

	//= Save/Load =

	virtual void					Serialize(YAML::Emitter& Archive) override;
	virtual void					Deserialize(YAML::Node& Archive)  override;

private:

	//= Private Structs =======================================

	struct LightCB
	{
		alignas(16) DirectX::XMFLOAT3	m_Position;
		alignas(16) DirectX::XMFLOAT3	m_Ambient;
		alignas(16) DirectX::XMFLOAT3	m_Diffuse;
		float							m_DiffuseIntensity;
		float							m_AttenuationConstant;
		float							m_AttenuationLinear;
		float							m_AttenuationQuadratic;
	};

private:

	//= Private Variables =====================================

	LightCB							m_CB;
	PixelConstantBuffer<LightCB>	m_PixelCB;
};