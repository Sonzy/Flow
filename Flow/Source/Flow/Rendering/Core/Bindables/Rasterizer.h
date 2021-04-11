#pragma once

// Includes /////////////////////////////////////////////////////////////////////

#include "Framework/Types/ComPtr.h"
#include "Rendering/Core/Bindables/Bindable.h"

// Forward Declarations /////////////////////////////////////////////////////////

struct ID3D11RasterizerState;

// Class Definition  ////////////////////////////////////////////////////////////

class Rasterizer : public Bindables::Bindable
{
public:

	// Public Enums  ////////////////////////////////////////////////////////////

	enum CullMode
	{
		Cull_None = 1,
		Cull_Front = 2,
		Cull_Back = 3
	};

public:

	// Public Static Functions  //////////////////////////////////////////////////

	static Rasterizer*						Resolve(CullMode mode);
	static HashString						GenerateID(CullMode mode);
	static const char*						CullModeToString(CullMode mode);

public:

	// Public Functions  /////////////////////////////////////////////////////////

											Rasterizer(CullMode mode);
	virtual void							Bind() override;


	//= Bindable Interface =

	virtual HashString						GetID();

private:

	// Private Variables  ////////////////////////////////////////////////////////

	ComPtr<ID3D11RasterizerState>			m_rasterizerState;
	CullMode								m_cullMode;
};