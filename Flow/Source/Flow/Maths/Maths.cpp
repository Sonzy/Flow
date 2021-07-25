#include "pch.h"
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
	return 	powf(V2.x - V1.x, 2) + powf(V2.y - V1.y, 2) + powf(V2.z - V1.z, 2);;
}

Rotator Maths::FindLookAtRotation(Vector3 StartPosition, Vector3 EndPosition)
{
	Rotator outRotation;
	Vector3 diff = EndPosition - StartPosition;

	outRotation.Pitch = atan2f(-diff.y, diff.z);

	if (diff.z >= 0) {
		outRotation.Yaw = -atan2f(diff.x * cosf(outRotation.Pitch), diff.z);
	}
	else {
		outRotation.Yaw = atan2f(diff.x * cosf(outRotation.Pitch), -diff.z);
	}

	return Rotator::AsDegrees(outRotation);
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

Rotator Maths::QuaternionToEulers(DirectX::XMVECTOR quaternion)
{
	//Function taken from https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles

	Rotator outRot;
	DirectX::XMFLOAT4 q;
	DirectX::XMStoreFloat4(&q, quaternion);

	// roll (x-axis rotation)
	double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
	double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
	outRot.Pitch = (float)std::atan2(sinr_cosp, cosr_cosp);

	// pitch (y-axis rotation)
	double sinp = 2 * (q.w * q.y - q.z * q.x);
	if (std::abs(sinp) >= 1)
		outRot.Yaw = (float)std::copysign(PI / 2, sinp); // use 90 degrees if out of range
	else
		outRot.Yaw = (float)std::asin(sinp);

	// yaw (z-axis rotation)
	double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
	double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
	outRot.Roll = (float)std::atan2(siny_cosp, cosy_cosp);

	return Rotator::AsDegrees(outRot);
}

DirectX::XMVECTOR Maths::EulersToQuaternion(Rotator rot)
{
	//Function taken from https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles

	// Abbreviations for the various angular functions
	double cy = cos(rot.Roll * 0.5);
	double sy = sin(rot.Roll * 0.5);
	double cp = cos(rot.Yaw * 0.5);
	double sp = sin(rot.Yaw * 0.5);
	double cr = cos(rot.Pitch * 0.5);
	double sr = sin(rot.Pitch * 0.5);

	DirectX::XMFLOAT4 q;
	q.w = (float)(cr * cp * cy + sr * sp * sy);
	q.x = (float)(sr * cp * cy - cr * sp * sy);
	q.y = (float)(cr * sp * cy + sr * cp * sy);
	q.z = (float)(cr * cp * sy - sr * sp * cy);

	return DirectX::XMLoadFloat4(&q);
}

bool Maths::CompareMatrices(DirectX::XMFLOAT4X4 m1, DirectX::XMFLOAT4X4 m2)
{
#define SHOW_EXACT_DIFFERENCES 1

#if SHOW_EXACT_DIFFERENCES
	if (CompareFloat(m1._11,m2._11) == false)
	{
		FLOW_ENGINE_LOG("_11 is different, %f %f", m1._11, m2._11);
		return false;
	}
		
	if (CompareFloat(m1._12, m2._12) == false)
	{
		FLOW_ENGINE_LOG("_12 is different, %f %f", m1._12, m2._12);
		return false;
	}

	if (CompareFloat(m1._13, m2._13) == false)
	{
		FLOW_ENGINE_LOG("_13 is different, %f %f", m1._13, m2._13);
		return false;
	}

	if (CompareFloat(m1._14, m2._14) == false)
	{
		FLOW_ENGINE_LOG("_14 is different, %f %f", m1._14, m2._14);
		return false;
	}



	if (CompareFloat(m1._21, m2._21) == false)
	{
		FLOW_ENGINE_LOG("_21 is different, %f %f", m1._21, m2._21);
		return false;
	}
	if (CompareFloat(m1._22, m2._22) == false)
	{
		FLOW_ENGINE_LOG("_22 is different, %f %f", m1._22, m2._22);
		return false;
	}
	if (CompareFloat(m1._23, m2._23) == false)
	{
		FLOW_ENGINE_LOG("_23 is different, %f %f", m1._23, m2._23);
		return false;
	}
	if (CompareFloat(m1._24, m2._24) == false)
	{
		FLOW_ENGINE_LOG("_24 is different, %f %f", m1._24, m2._24);
		return false;
	}


	if (CompareFloat(m1._31, m2._31) == false)
	{
		FLOW_ENGINE_LOG("_31 is different, %f %f", m1._31, m2._31);
		return false;
	}

	if (CompareFloat(m1._32, m2._32) == false)
	{
		FLOW_ENGINE_LOG("_32 is different, %f %f", m1._32, m2._32);
		return false;
	}

	if (CompareFloat(m1._33, m2._33) == false)
	{
		FLOW_ENGINE_LOG("_33 is different, %f %f", m1._33, m2._33);
		return false;
	}

	if (CompareFloat(m1._34, m2._34) == false)
	{
		FLOW_ENGINE_LOG("_34 is different, %f %f", m1._34, m2._34);
		return false;
	}



	if (CompareFloat(m1._41, m2._41) == false)
	{
		FLOW_ENGINE_LOG("_41 is different, %f %f", m1._41, m2._41);
		return false;
	}

	if (CompareFloat(m1._42, m2._42) == false)
	{
		FLOW_ENGINE_LOG("_42 is different, %f %f", m1._42, m2._42);
		return false;
	}

	if (CompareFloat(m1._43, m2._43) == false)
	{
		FLOW_ENGINE_LOG("_43 is different, %f %f", m1._43, m2._43);
		return false;
	}

	if (CompareFloat(m1._44, m2._44) == false)
	{
		FLOW_ENGINE_LOG("_44 is different, %f %f", m1._44, m2._44);
		return false;
	}

	return true;
#else
	return
		m1._11 == m2._11 &&
		m1._12 == m2._12 &&
		m1._13 == m2._13 &&
		m1._14 == m2._14 &&

		m1._21 == m2._21 &&
		m1._22 == m2._22 &&
		m1._23 == m2._23 &&
		m1._24 == m2._24 &&

		m1._31 == m2._31 &&
		m1._32 == m2._32 &&
		m1._33 == m2._33 &&
		m1._34 == m2._34 &&

		m1._41 == m2._41 &&
		m1._42 == m2._42 &&
		m1._43 == m2._43 &&
		m1._44 == m2._44;
#endif

}

#endif //#if USE_DXMATH_FORMULAE

void Maths::PrintMatrix(float* m)
{
	FLOW_ENGINE_LOG("%f %f %f %f", m[0], m[1], m[2], m[3]);
	FLOW_ENGINE_LOG("%f %f %f %f", m[4], m[5], m[6], m[7]);
	FLOW_ENGINE_LOG("%f %f %f %f", m[8], m[9], m[10], m[11]);
	FLOW_ENGINE_LOG("%f %f %f %f", m[12], m[13], m[14], m[15]);
}

void Maths::PrintMatrix3x3(float* m)
{
	FLOW_ENGINE_LOG("%f %f %f", m[0], m[1], m[2]);
	FLOW_ENGINE_LOG("%f %f %f", m[3], m[4], m[5]);
	FLOW_ENGINE_LOG("%f %f %f", m[6], m[7], m[8]);
}

void Maths::PrintQuaternion(float* v)
{
	FLOW_ENGINE_LOG("%f %f %f %f", v[0], v[1], v[2], v[3]);
}

bool Maths::CompareFloat(float a, float b, float epsilon)
{
	return abs(b - a) < epsilon;
}

Vector3 Maths::Transform(DirectX::XMMATRIX matrix, Vector3 position)
{
	DirectX::XMVECTOR vec = DirectX::XMLoadFloat3(reinterpret_cast<DirectX::XMFLOAT3*>(&position));
	DirectX::XMVECTOR transformed = DirectX::XMVector3Transform(vec, matrix);

	DirectX::XMFLOAT3 out{};
	DirectX::XMStoreFloat3(&out, transformed);
	return out;
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
