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

	// Public Functions ///////////////////////////////////////////

								SpriteComponent();
								SpriteComponent(const std::string& Name);
								~SpriteComponent();

	virtual void				RefreshBinds() override;
	virtual void				BeginPlay() override;
	virtual void				Tick(float DeltaTime) override;
	virtual void				Render() override;
	virtual void				InitialisePhysics() override;

#if WITH_EDITOR
	virtual void				EditorBeginPlay() override;
	virtual void				OnViewportSelected() override;
	virtual void				OnViewportDeselected() override;

	virtual void				DrawComponentDetailsWindow() override;
#endif

	//= Saving and Loading =

	virtual std::string			GetClassSerializationUID(std::ofstream* Archive) override;
	virtual void				Serialize(std::ofstream* Archive) override;
	virtual void				Deserialize(std::ifstream* Archive, Actor* NewParent) override;

private:

	// Private Functions //////////////////////////////////////////

	void						GenerateCollision();

private:

	// Private Variables //////////////////////////////////////////

	//TODO:	//Texture*					m_Sprite;
	Vector4						m_Tint;
	Material*					m_Material;
	bool						m_DoubleSided;
	bool						m_Billboard;
};