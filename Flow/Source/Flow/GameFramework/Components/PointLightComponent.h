#pragma once

//= Includes ==============================================================

#include "Flow/GameFramework/Components/RenderableComponent.h"
#include "Flow\Rendering\Core\Bindables\ConstantBuffers\ShaderConstantBuffers.h"

//= Class Definitions =====================================================


class PointLightComponent : public RenderableComponent
{
public:

	//= Public Functions ======================================

										PointLightComponent();
										PointLightComponent(const std::string& ComponentName);

	virtual void						Render() override;
	virtual void						DrawComponentDetailsWindow() override;

	virtual std::string					GetClassSerializationUID(std::ofstream* Archive);
	virtual void						Serialize(std::ofstream* Archive) override;
	virtual void						Deserialize(std::ifstream* Archive, Actor* NewParent) override;

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