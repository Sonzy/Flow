#pragma once
#include "Assets/Textures/TextureAsset.h"
#include "GameFramework/Components/WorldComponent.h"
#include "Flow/Rendering/Core/Renderable.h"
#include "Typedefs.h"
#include "Rendering/Core/Materials/Material.h"

// Class Definition ////////////////////////////////////////////

class Icon;

class IconManager
{
public:

	// Public Structs /////////////////////////////

	struct IconData
	{
		TextureAsset* m_texture;
	};

public:
	// Public Functions ////////////////////////////

	IconManager();

	void								RegisterIcon(FGUID guid, const IconData& data);
	void								RemoveIcon(FGUID guid);

	void								RenderIcons();

	const Material&						GetIconMaterial() const;

	void								RenderDebugWindow();

	//TODO: Debug
	Rotator									m_rotatorOverride;
	Vector3									m_posOverride;
	bool									m_InverseViewMatrix = false;
	bool									m_NoViewMatrix = false;

private:
	std::unordered_map<FGUID, Icon*>		m_iconData;
	Material*								m_iconMaterial;

};

class Icon : public Renderable
{
public:
	// Public Functions ////////////////////////////

	Icon(FGUID guid, TextureAsset* tex);

	void							RefreshBinds(const IconManager& manager);
	virtual DirectX::XMMATRIX		GetTransformXM() const;

	//TODO: store a pointer to the parent?
	TextureAsset*					m_texture;
	FGUID							m_guid;
	bool							m_doubleSided; //TODO: Implement

	Vector3							m_position;
	Rotator							m_rotation;
	Vector2							m_scale;
	//TODO: Face the camera
};

