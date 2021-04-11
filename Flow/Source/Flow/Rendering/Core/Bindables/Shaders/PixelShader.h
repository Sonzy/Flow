#pragma once

// Includes //////////////////////////////////////////////////////////

#include "Framework/Types/ComPtr.h"
#include "Rendering/Core/Bindables/Bindable.h"

// Forward Declarations /////////////////////////////////////////////
 
 struct ID3D11PixelShader;
 
// Namespace ////////////////////////////////////////////////////////

 namespace Bindables
 {
	 // Class Definition /////////////////////////////////////////////////

	 class PixelShader : public Bindables::Bindable
	 {
	 public:

		 // Public Static Functions //////////////////////////////////////

		 static PixelShader*						Resolve(const string& localPath);
		 static HashString							GenerateID(const string& localPath);

	 public:

		 // Public Functions ////////////////////////////////////////////

													PixelShader(const string& localPath);
		 void										Bind() override;
		 virtual HashString							GetID() override;

	 private:

		 // Private Variables ///////////////////////////////////////////

		 string										m_shaderPath;
		 ComPtr<ID3D11PixelShader>					m_pixelShader;
	 };
 }