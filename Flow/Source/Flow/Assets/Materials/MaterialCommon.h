#pragma once
#include "Typedefs.h"

// Namespace Definition /////////////////////////////////////
namespace MaterialCommon
{
	// Macros ///////////////////////////////////////////////

	// Constants ////////////////////////////////////////////

	namespace Register
	{
		static uint32 ObjectLightProperties = 1;
		static uint32 ColorFloat4 = 2;
	}

	namespace Buffer
	{


		struct F32F32F32F32
		{
			float x;
			float y;
			float z;
			float w;
		};

		struct ColorFloat4
		{
			float r;
			float g;
			float b;
			float a;
		};

		struct ObjectLightProperties
		{
			DirectX::XMFLOAT3					m_SpecularColour;
			float								m_SpecularWeight;
			float								m_SpecularGloss;
			float								m_Padding[3];
		};

		struct BasicLight
		{
			alignas(16) DirectX::XMFLOAT3		m_Position;
			alignas(16) DirectX::XMFLOAT3		m_Ambient;
			alignas(16) DirectX::XMFLOAT3		m_Diffuse;
			float								m_DiffuseIntensity;
			float								m_AttenuationConstant;
			float								m_AttenuationLinear;
			float								m_AttenuationQuadratic;
		};
	}
}
