#include "Flowpch.h"
#include "Frustrum.h"

namespace Flow
{
	void Frustrum::UpdateFrustrum(float Depth, DirectX::XMMATRIX Projection, DirectX::XMMATRIX View)
	{
		DirectX::XMMATRIX Matrix;
		DirectX::XMFLOAT4X4 ProjMatrix;
		DirectX::XMStoreFloat4x4(&ProjMatrix, Projection);

		//Calculate the min Z distance
		float ZMinimum = -ProjMatrix._43 / ProjMatrix._33;
		float r = Depth / (Depth - ZMinimum);
		ProjMatrix._33 = r;
		ProjMatrix._43 = -r * ZMinimum;

		//Create the frustrum matrix from the updated matrices
		Matrix = DirectX::XMMatrixMultiply(View, Projection);
		DirectX::XMFLOAT4X4 FMatrix;
		DirectX::XMStoreFloat4x4(&FMatrix,Matrix);

		DirectX::XMFLOAT4 Plane;
		// Calculate near plane of frustum.
		DirectX::XMStoreFloat4(&Plane, m_Planes[0]);
		Plane.w = FMatrix._14 + FMatrix._13;
		Plane.x = FMatrix._24 + FMatrix._23;
		Plane.y = FMatrix._34 + FMatrix._33;
		Plane.z = FMatrix._44 + FMatrix._43;
		m_Planes[0] = DirectX::XMLoadFloat4(&Plane);
		DirectX::XMPlaneNormalize(m_Planes[0]);

		// Calculate far plane of frustum.
		DirectX::XMStoreFloat4(&Plane, m_Planes[1]);
		Plane.w = FMatrix._14 - FMatrix._13;
		Plane.x = FMatrix._24 - FMatrix._23;
		Plane.y = FMatrix._34 - FMatrix._33;
		Plane.z = FMatrix._44 - FMatrix._43;
		m_Planes[1] = DirectX::XMLoadFloat4(&Plane);
		DirectX::XMPlaneNormalize(m_Planes[1]);

		// Calculate left plane of frustum.
		DirectX::XMStoreFloat4(&Plane, m_Planes[2]);
		Plane.w = FMatrix._14 + FMatrix._11;
		Plane.x = FMatrix._24 + FMatrix._21;
		Plane.y = FMatrix._34 + FMatrix._31;
		Plane.z = FMatrix._44 + FMatrix._41;
		m_Planes[2] = DirectX::XMLoadFloat4(&Plane);
		DirectX::XMPlaneNormalize(m_Planes[2]);

		// Calculate right plane of frustum.
		DirectX::XMStoreFloat4(&Plane, m_Planes[3]);
		Plane.w = FMatrix._14 - FMatrix._11;
		Plane.x = FMatrix._24 - FMatrix._21;
		Plane.y = FMatrix._34 - FMatrix._31;
		Plane.z = FMatrix._44 - FMatrix._41;
		m_Planes[3] = DirectX::XMLoadFloat4(&Plane);
		DirectX::XMPlaneNormalize(m_Planes[3]);

		// Calculate top plane of frustum.
		DirectX::XMStoreFloat4(&Plane, m_Planes[4]);
		Plane.w = FMatrix._14 - FMatrix._12;
		Plane.x = FMatrix._24 - FMatrix._22;
		Plane.y = FMatrix._34 - FMatrix._32;
		Plane.z = FMatrix._44 - FMatrix._42;
		m_Planes[4] = DirectX::XMLoadFloat4(&Plane);
		DirectX::XMPlaneNormalize(m_Planes[4]);

		// Calculate bottom plane of frustum.
		DirectX::XMStoreFloat4(&Plane, m_Planes[5]);
		Plane.w = FMatrix._14 + FMatrix._12;
		Plane.x = FMatrix._24 + FMatrix._22;
		Plane.y = FMatrix._34 + FMatrix._32;
		Plane.z = FMatrix._44 + FMatrix._42;
		m_Planes[5] = DirectX::XMLoadFloat4(&Plane);
		DirectX::XMPlaneNormalize(m_Planes[5]);
	}
	bool Frustrum::CheckCube(Vector Position, float Radius) const
	{
		int i;

		DirectX::XMVECTOR Location;
		DirectX::XMFLOAT3 Dot;

		for (i = 0; i < 6; i++)
		{
			Location = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3((Position.X - Radius), (Position.Y - Radius), (Position.Z - Radius)));
			DirectX::XMStoreFloat3(&Dot, DirectX::XMPlaneDotCoord(m_Planes[i], Location));
			if (Dot.x >= 0.0f)
				continue;

			Location = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3((Position.X + Radius), (Position.Y - Radius), (Position.Z - Radius)));
			DirectX::XMStoreFloat3(&Dot, DirectX::XMPlaneDotCoord(m_Planes[i], Location));
			if (Dot.x >= 0.0f)
				continue;

			Location = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3((Position.X - Radius), (Position.Y + Radius), (Position.Z - Radius)));
			DirectX::XMStoreFloat3(&Dot, DirectX::XMPlaneDotCoord(m_Planes[i], Location));
			if (Dot.x >= 0.0f)
				continue;

			Location = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3((Position.X + Radius), (Position.Y + Radius), (Position.Z - Radius)));
			DirectX::XMStoreFloat3(&Dot, DirectX::XMPlaneDotCoord(m_Planes[i], Location));
			if (Dot.x >= 0.0f)
				continue;

			Location = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3((Position.X - Radius), (Position.Y - Radius), (Position.Z + Radius)));
			DirectX::XMStoreFloat3(&Dot, DirectX::XMPlaneDotCoord(m_Planes[i], Location));
			if (Dot.x >= 0.0f)
				continue;

			Location = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3((Position.X + Radius), (Position.Y - Radius), (Position.Z + Radius)));
			DirectX::XMStoreFloat3(&Dot, DirectX::XMPlaneDotCoord(m_Planes[i], Location));
			if (Dot.x >= 0.0f)
				continue;

			Location = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3((Position.X - Radius), (Position.Y + Radius), (Position.Z + Radius)));
			DirectX::XMStoreFloat3(&Dot, DirectX::XMPlaneDotCoord(m_Planes[i], Location));
			if (Dot.x >= 0.0f)
				continue;

			Location = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3((Position.X + Radius), (Position.Y + Radius), (Position.Z + Radius)));
			DirectX::XMStoreFloat3(&Dot, DirectX::XMPlaneDotCoord(m_Planes[i], Location));
			if (Dot.x >= 0.0f)
				continue;

			return false;
		}

		return true;
	}
}