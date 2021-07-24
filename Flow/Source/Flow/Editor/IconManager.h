#pragma once

// Includes ////////////////////////////////////////////////////

#include <vector>
#include "Typedefs.h"
#include "Assets/Textures/TextureAsset.h"
#include "Flow/Rendering/Core/Renderable.h"
#include "GameFramework/Components/WorldComponent.h"
#include "Rendering/Core/Materials/Material.h"
#include "Rendering/Core/Vertex/VertexBufferData.h"
#include "Rendering/Core/Bindables/ConstantBuffers/ShaderConstantBuffers.h"

// Class Definition ////////////////////////////////////////////

class IconManager;

class Icon : public Renderable
{
public:

	struct IconVertexData
	{
		float xPosition;
		float yPosition;
		Vector2 windowSize;

		float xScale;
		float yScale;
		float padding1;
		float padding2;
	};

	struct IconPixelData
	{
		Vector4 m_tint = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	};

	enum class Alignment
	{
		TopLeft = 0,
		Centre = 1
	};

public:
	// Public Functions ////////////////////////////

	Icon(FGuid guid, TextureAsset* tex);

	void							RefreshBinds(const IconManager& manager);
	virtual DirectX::XMMATRIX		GetTransformXM() const override;

	FGuid							m_guid;
	Vector3							m_position;
	Icon::Alignment					m_alignment;
	Vector4							m_tint;

	IconVertexData					m_buf;
	IconPixelData					m_pBuf;
	VertexConstantBuffer<IconVertexData>* m_vCB;
	PixelConstantBuffer<IconPixelData>* m_pCB;

	Material						m_iconMaterial;
};

class IconManager : public UIComponent
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

	void								RegisterIcon(FGuid guid, const IconData& data);
	void								RemoveIcon(FGuid guid);
	void								Reset();

	virtual void						Update() override;
	virtual void						Render() override;

	void								RenderIcons();

	static const Material*				GetIconMaterial();

	const VertexBuffer&					GetIconVertices(Icon::Alignment alignment) const;
	const std::vector<uint16>&			GetIconIndices() const;
	const VertexLayout&					GetIconLayout() const;

	Icon&								GetIcon(FGuid iconGuid);
	float								GetIconSize() const;

private:
	static Material*						sm_iconMaterialDefault;

	std::unordered_map<FGuid, Icon*>		m_iconData;

	VertexBuffer							m_iconVerticesTopLeftAligned;
	VertexBuffer							m_iconVerticesCentreAligned;
	std::vector<uint16>						m_iconIndices;
	VertexLayout							m_iconLayout;

	//= Configuration =

	float									m_iconSize;

	//= Debug =

	bool									m_showDebugWindow;
};