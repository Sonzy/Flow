#include "Flowpch.h"
#include "Maths.h"
#include "Physics/Physics.h"

Rotator Maths::DegreesToRadians(const Rotator& Source)
{
	return Source * (PI / 180.0f);
}

float Maths::DegreesToRadians(float Source)
{
	return Source * (PI / 180.0f);
}

Rotator Maths::RadiansToDegrees(const Rotator& Source)
{
	return Source * (180.0f / PI);
}

float Maths::RadiansToDegrees(float Source)
{
	return Source * (180.0f / PI);
}

Vector3 Maths::GetClosestDistanceBetweenLines(const Physics::Ray& R1, const Physics::Ray& R2, float& OutR1Scale, float& OutR2Scale)
{
	Vector3 Distance = R2.m_Origin - R1.m_Origin;
	const float v12 = Vector3::Dot(R1.m_Direction, R1.m_Direction);
	const float v22 = Vector3::Dot(R2.m_Direction, R2.m_Direction);
	const float v1v2 = Vector3::Dot(R1.m_Direction, R2.m_Direction);

	const float det = v1v2 * v1v2 - v12 * v22;

	if (std::abs(det) > FLT_MIN)
	{
		const float inv_det = 1.f / det;

		const float dpv1 = Vector3::Dot(Distance, R1.m_Direction);
		const float dpv2 = Vector3::Dot(Distance, R2.m_Direction);

		OutR1Scale = inv_det * (v22 * dpv1 - v1v2 * dpv2);
		OutR2Scale = inv_det * (v1v2 * dpv1 - v12 * dpv2);

		return Vector3(Distance + R2.m_Direction * OutR2Scale - R1.m_Direction * OutR1Scale).Normalize();
	}
	else
	{
		const Vector3 a = Vector3::Cross(Distance, R1.m_Direction);
		return std::sqrt(Vector3::Dot(a, a) / v12);
	}
}

float Maths::Distance(Vector3 V1, Vector3 V2)
{
	return 	sqrtf(DistanceSquared(V1, V2));
}

float Maths::DistanceSquared(Vector3 V1, Vector3 V2)
{
	return 	pow(V2.x - V1.x, 2) + pow(V2.y - V1.y, 2) + pow(V2.z - V1.z, 2);;
}

Rotator Maths::FindLookAtRotation(Vector3 StartPosition, Vector3 EndPosition)
{
	//TODO: Make it work lmao
	float xdis = EndPosition.x - StartPosition.x;
	float ydis = EndPosition.z - StartPosition.z;
	float zdis = EndPosition.y - StartPosition.y;
	float xzdis = sqrtf(xdis * xdis + zdis * zdis);

	return Rotator(RadiansToDegrees(-atan2f(ydis, xzdis)), RadiansToDegrees(-(atan2f(-xdis, zdis))), 0);
}

#if USE_DXMATH_FORMULAE

void Maths::DecomposeMatrix(Vector3& translation, Rotator& rotation, Vector3& scale, DirectX::XMMATRIX matrix)
{
	DirectX::XMVECTOR vtranslation;
	DirectX::XMVECTOR vrotation;
	DirectX::XMVECTOR vscale;
	DirectX::XMMatrixDecompose(&vtranslation, &vrotation, &vscale, matrix);

	translation = Vector3(vtranslation.m128_f32[0], vtranslation.m128_f32[1], vtranslation.m128_f32[2]);
	rotation = Rotator(vrotation.m128_f32[0], vrotation.m128_f32[1], vrotation.m128_f32[2]);
	scale = Vector3(vscale.m128_f32[0], vscale.m128_f32[1], vscale.m128_f32[2]);
}

void Maths::PrintMatrix(DirectX::XMMATRIX m)
{
	DirectX::XMFLOAT4X4 fMatrix;
	DirectX::XMStoreFloat4x4(&fMatrix, m);

	FLOW_ENGINE_LOG("%f %f %f %f", fMatrix._11, fMatrix._12, fMatrix._13, fMatrix._14);
	FLOW_ENGINE_LOG("%f %f %f %f", fMatrix._21, fMatrix._22, fMatrix._23, fMatrix._24);
	FLOW_ENGINE_LOG("%f %f %f %f", fMatrix._31, fMatrix._32, fMatrix._33, fMatrix._34);
	FLOW_ENGINE_LOG("%f %f %f %f", fMatrix._41, fMatrix._42, fMatrix._43, fMatrix._44);
}

#endif //#if USE_DXMATH_FORMULAE

void Maths::PrintMatrix(float* m)
{
	FLOW_ENGINE_LOG("%f %f %f %f", m[0], m[1], m[2], m[3]);
	FLOW_ENGINE_LOG("%f %f %f %f", m[4], m[5], m[6], m[7]);
	FLOW_ENGINE_LOG("%f %f %f %f", m[8], m[9], m[10], m[11]);
	FLOW_ENGINE_LOG("%f %f %f %f", m[12], m[13], m[14], m[15]);
}

FLOW_API Vector3::Vector3(Vector4 v)
	: x(v.x), y(v.y), z(v.z)
{}

FLOW_API Vector3::Vector3(IntVector3 v)
	: x(static_cast<float>(v.x)), y(static_cast<float>(v.y)), z(static_cast<float>(v.z))
{}

FLOW_API Vector2::Vector2(Vector3 v)
	: x(v.x), y(v.y)
{}

FLOW_API Vector2::Vector2(IntVector2 v)
	: x(static_cast<float>(v.x)), y(static_cast<float>(v.y))
{}

FLOW_API Vector3::Vector3(IntVector2 v)
	: x(static_cast<float>(v.x)), y(static_cast<float>(v.y)), z(0)
{}

FLOW_API Vector4::Vector4(IntVector4 v)
	: x(static_cast<float>(v.x)), y(static_cast<float>(v.z)), z(static_cast<float>(v.y)), w(static_cast<float>(v.w))
{}
