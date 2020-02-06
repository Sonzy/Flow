#pragma once
#include <DirectXMath.h>
#include "Flow\Helper\Maths.h"

namespace Flow
{
	class Frustrum
	{
	public:
		Frustrum() {};

		void UpdateFrustrum(float Depth, DirectX::XMMATRIX Projection, DirectX::XMMATRIX View);

		bool CheckCube(Vector Position, float Radius) const;

	private:
		DirectX::XMVECTOR m_Planes[6];
	};
}