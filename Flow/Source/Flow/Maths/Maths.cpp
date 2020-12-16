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
	: x(v.x), y(v.y), z(v.z), w(v.w)
{}
