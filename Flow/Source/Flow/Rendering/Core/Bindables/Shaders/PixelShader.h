#pragma once

// Includes //////////////////////////////////////////////////////////

#include <string>
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

		 static PixelShader*						Resolve(const std::string& localPath);
		 static HashString							GenerateID(const std::string& localPath);

	 public:

		 // Public Functions ////////////////////////////////////////////

													PixelShader(const std::string& localPath);
		 void										Bind() override;
		 virtual HashString							GetID() override;

	 private:

		 // Private Variables ///////////////////////////////////////////

		 std::string								m_shaderPath;
		 ComPtr<ID3D11PixelShader>					m_pixelShader;
	 };
 }