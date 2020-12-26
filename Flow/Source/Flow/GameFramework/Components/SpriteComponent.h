#pragma once

// Includes ///////////////////////////////////////////////////////

#include "RenderableComponent.h"

// Forward Declarations ///////////////////////////////////////////

class Texture;
class Material;

// Class Definition ///////////////////////////////////////////////

//TODO: Collision for sprite component
class SpriteComponent : public RenderableComponent
{
public:

	REGISTER_CLASS(SpriteComponent)

	// Public Functions ///////////////////////////////////////////

								SpriteComponent();
								SpriteComponent(const std::string& Name);
								~SpriteComponent();

	virtual void				RefreshBinds() override;
	virtual void				BeginPlay() override;
	virtual void				Render() override;
	virtual void				InitialisePhysics() override;

#if WITH_EDITOR
	virtual void				EditorBeginPlay() override;
	virtual void				OnViewportSelected() override;
	virtual void				OnViewportDeselected() override;

	virtual void				DrawComponentDetailsWindow() override;
#endif

	//= Saving and Loading =

	virtual void				Serialize(YAML::Emitter& Archive) override;
	virtual void				Deserialize(YAML::Node& Archive) override;

private:

	// Private Functions //////////////////////////////////////////

	void						GenerateCollision();

private:

	// Private Variables //////////////////////////////////////////

	//TODO:	//Texture*					m_Sprite;
	Vector4						m_Tint;
	Material*					m_Material;
	bool						m_DoubleSided;
};