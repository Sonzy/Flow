#pragma once
#include <DirectXMath.h>
#include "Maths/Vector2.h"
#include "Maths/Vector3.h"
#include "Maths/Vector4.h"

namespace DirectX
{
	DirectX::XMFLOAT2 Convert(Vector2 Other)
	{
		return DirectX::XMFLOAT2(Other.x, Other.y);
	}

	DirectX::XMFLOAT3 Convert(Vector3 Other)
	{
		return DirectX::XMFLOAT3(Other.x, Other.y, Other.z);
	}

	DirectX::XMFLOAT4 Convert(Vector4 Other)
	{
		return DirectX::XMFLOAT4(Other.x, Other.y, Other.z, Other.w);
	}
}