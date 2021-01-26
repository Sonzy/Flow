#pragma once
#include "Assets/Textures/TextureAsset.h"
#include "GameFramework/Components/WorldComponent.h"
#include "Flow/Rendering/Core/Renderable.h"
#include "Typedefs.h"
#include "Rendering/Core/Materials/Material.h"
#include "Rendering/Core/Vertex/VertexBuffer.h"
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

	Icon(FGUID guid, TextureAsset* tex);

	void							RefreshBinds(const IconManager& manager);
	virtual DirectX::XMMATRIX		GetTransformXM() const override;

	FGUID							m_guid;
	Vector3							m_position;
	Icon::Alignment					m_alignment;
	Vector4							m_tint;

	IconVertexData					m_buf;
	IconPixelData					m_pBuf;
	VertexConstantBuffer<IconVertexData>* m_vCB;
	PixelConstantBuffer<IconPixelData>* m_pCB;
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

	void								RegisterIcon(FGUID guid, const IconData& data);
	void								RemoveIcon(FGUID guid);

	virtual void						Update() override;
	virtual void						Render() override;

	void								RenderIcons();

	const Material&						GetIconMaterial() const;

	const VertexBuffer&					GetIconVertices(Icon::Alignment alignment) const;
	const std::vector<unsigned short>&	GetIconIndices() const;
	const VertexLayout&					GetIconLayout() const;

	Icon&								GetIcon(FGUID iconGuid);
	float								GetIconSize() const;

private:
	std::unordered_map<FGUID, Icon*>		m_iconData;
	Material*								m_iconMaterial;

	VertexBuffer							m_iconVerticesTopLeftAligned;
	VertexBuffer							m_iconVerticesCentreAligned;
	std::vector<unsigned short>				m_iconIndices;
	VertexLayout							m_iconLayout;

	//= Configuration =

	float									m_iconSize;

	//= Debug =

	bool									m_showDebugWindow;
};