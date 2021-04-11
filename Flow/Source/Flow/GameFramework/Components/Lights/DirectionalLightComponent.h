#pragma once

// Includes ////////////////////////////////////////////////////////

#include "GameFramework/Components/RenderableComponent.h"
#include "Rendering/Core/Bindables/ConstantBuffers/ShaderConstantBuffers.h"
#include "Assets/Materials/MaterialCommon.h"

// Type Definitions ////////////////////////////////////////////////////////

typedef MaterialCommon::Buffer::DirectionalLight DirectionalLightBuffer_t;

// Forward Declarations ///////////////////////////////////////////////////

class StaticMesh;

// Class Definition ////////////////////////////////////////////////////////

class DirectionalLightComponent : public RenderableComponent
{
public:

	REGISTER_CLASS(DirectionalLightComponent)

		// Public Functions ////////////////////////////////////////////////////////

	DirectionalLightComponent();
	DirectionalLightComponent(const std::string& ComponentName);

	virtual void						OnRegistered() override;
	virtual void						Render() override;
	virtual void						DrawComponentDetailsWindow() override;

#if WITH_EDITOR
	virtual void						IconUpdate(IconManager& iconManager) override;
#endif

	const DirectionalLightBuffer_t&		GetLightBuffer() const;
	void								SetLightBuffer(const DirectionalLightBuffer_t& buf);

	//= Save/Load =

	virtual void						Serialize(YAML::Emitter& Archive) override;
	virtual void						Deserialize(YAML::Node& Archive)  override;

private:

	// Private Variables ////////////////////////////////////////////////////////

	DirectionalLightBuffer_t								m_lightBuffer;
	PixelConstantBuffer<DirectionalLightBuffer_t>			m_lightPixelBuffer;

#if WITH_EDITOR
	StaticMesh*												m_arrowPointer;
#endif
};