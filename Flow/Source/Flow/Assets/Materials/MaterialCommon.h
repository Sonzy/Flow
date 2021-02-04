#pragma once
#include "Typedefs.h"

// Namespace Definition /////////////////////////////////////
namespace MaterialCommon
{
	// Macros ///////////////////////////////////////////////

	// Constants ////////////////////////////////////////////

	// PixelConstantBuffers
	namespace Register
	{
		static uint8 Selection = 10;
		static uint8 ObjectLightProperties = 11;
		static uint8 PointLightProperties = 12;
		static uint8 DirectionalLightProperties = 13;
		static uint8 Camera = 9;
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
			alignas(16) DirectX::XMFLOAT3		m_Diffuse;
			float								m_DiffuseIntensity;
			float								m_specularIntensity;
			float								m_attenuationConstant;
			float								m_attenuationLinear;
			float								m_attenuationQuadratic;
		};

		struct DirectionalLight
		{
			alignas(16) DirectX::XMFLOAT3		m_direction;
			alignas(16) DirectX::XMFLOAT3		m_ambientColor;
			alignas(16) DirectX::XMFLOAT3		m_diffuseColor;
			alignas(16) DirectX::XMFLOAT3		m_specularColor;
			float								m_diffuseIntensity;
			float								m_ambientIntensity;
			float								m_specularIntensity;
			float								padding;

			DirectionalLight()
				: m_direction(0.25f, -0.5f, 0.0f)
				, m_ambientColor(0.1f, 0.1f, 0.1f)
				, m_diffuseColor(1.0f, 1.0f, 1.0f)
				, m_specularColor(1.0f, 1.0f, 1.0f)
				, m_diffuseIntensity(0.75f)
				, m_ambientIntensity(0.1f)
				, m_specularIntensity(0.5f)
			{

			}
		};
	}
}
