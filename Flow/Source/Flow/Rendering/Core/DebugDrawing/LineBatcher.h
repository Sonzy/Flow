#pragma once

//= Includes ===================================================

#include "Rendering/Core/Renderable.h"
#include "Rendering/Core/Bindables/ConstantBuffers/ShaderConstantBuffers.h"

//= Forward Declarations =======================================

class VertexBuffer;
class BindableVertexBuffer;
class VertexLayout;

//= Class Definition ===========================================

class LineBatcher : public Renderable
{
public:

	struct Line
	{
		Vector3 From;
		Vector3 To;
		Vector3 Color;
	};

	//= Public Functions =======================================================

											LineBatcher();
											~LineBatcher();
	void									Initialise();
	void									AddLine(Vector3 From, Vector3 To, Vector3 Colour = Vector3(1.0f, 1.0f, 1.0f));
	void									AddPersistentLine(const std::string& tag, Vector3 From, Vector3 To, Vector3 Colour = Vector3(1.0f, 1.0f, 1.0f));
	void									DrawLines();
	void									FlushLines();
	unsigned int							GetNumberOfLines() const { return m_Lines; }
	DirectX::XMMATRIX						GetTransformXM() const override { return DirectX::XMMATRIX(); };

private:

	//= Private Structs =========================================================

	struct ViewProjectionBuffer
	{
		DirectX::XMMATRIX _ViewProjectionMatrix;
	};

	//= Private Functions =======================================================

	void											BindAll();
	void											AddBind(Bindable* NewBind);
	 
	//= Private Variables =======================================================

	std::vector<Bindable*>			m_Binds;													
	VertexBuffer*									m_VertexBuffer;
	BindableVertexBuffer*							m_BindableVertexBuffer;
	VertexLayout*									m_VertexLayout;													
	ViewProjectionBuffer							m_CameraMatrix;
	VertexConstantBuffer<ViewProjectionBuffer>*		m_VertexCB;
	unsigned int									m_Lines;

	std::unordered_map<std::string, Line>			m_PersistentLines;
};