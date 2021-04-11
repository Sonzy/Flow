#pragma once

// Includes ////////////////////////////////////////////////////////

#include "GameFramework/Components/RenderableComponent.h"
#include "Rendering/Core/Bindables/ConstantBuffers/ShaderConstantBuffers.h"
#include "Assets/Materials/MaterialCommon.h"

// Type Definitions ////////////////////////////////////////////////////////

typedef MaterialCommon::Buffer::BasicLight LightBuffer_t;

// Class Definition ////////////////////////////////////////////////////////

class PointLightComponent : public RenderableComponent
{
public:

	REGISTER_CLASS(PointLightComponent)

	// Public Functions ////////////////////////////////////////////////////////

										PointLightComponent();
										PointLightComponent(const std::string& ComponentName);

	virtual void						OnRegistered() override;
	virtual void						Render() override;
	virtual void						DrawComponentDetailsWindow() override;

#if WITH_EDITOR
	virtual void						IconUpdate(IconManager& iconManager) override;
#endif

	const LightBuffer_t&				GetLightBuffer() const;
	void								SetLightBuffer(const LightBuffer_t& buf);

	//= Save/Load =

	virtual void						Serialize(YAML::Emitter& Archive) override;
	virtual void						Deserialize(YAML::Node& Archive)  override;

private:

	// Private Variables ////////////////////////////////////////////////////////

	LightBuffer_t								m_lightBuffer;
	PixelConstantBuffer<LightBuffer_t>			m_lightPixelBuffer;
};