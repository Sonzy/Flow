#pragma once
#include "Flow/GameFramework/Components/RenderableComponent.h"
#include "Flow\Rendering\Core\Bindables\ConstantBuffers\ShaderConstantBuffers.h"

class PointLightComponent : public RenderableComponent
{
public:
	PointLightComponent();
	PointLightComponent(const std::string& ComponentName);

	virtual void Render() override;
	virtual void DrawComponentDetailsWindow() override;

private:
	struct LightCB
	{
		alignas(16) DirectX::XMFLOAT3 _Position;
		alignas(16) DirectX::XMFLOAT3 _Ambient;
		alignas(16) DirectX::XMFLOAT3 _Diffuse;
		float _DiffuseIntensity;
		float _AttenuationConstant;
		float _AttenuationLinear;
		float _AttenuationQuadratic;
	};

	LightCB _CB;
	PixelConstantBuffer<LightCB> _PixelCB;
};