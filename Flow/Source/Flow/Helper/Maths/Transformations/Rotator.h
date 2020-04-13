#pragma once
#include <DirectXMath.h>

//TODO: Simplify wrapping, was lazy
struct Rotator
{
	float Pitch;
	float Roll;
	float Yaw;

	Rotator()
		: Pitch(0), Roll(0), Yaw(0)
	{

	}

	Rotator(float Pitch, float Roll, float Yaw)
		: Pitch(Pitch), Roll(Roll), Yaw(Yaw)
	{

	}

	Rotator(float PitchRollYaw)
		: Pitch(PitchRollYaw), Roll(PitchRollYaw), Yaw(PitchRollYaw)
	{

	}

	void operator+=(const Rotator& Other)
	{
		Pitch += Other.Pitch;
		Roll += Other.Roll;
		Yaw += Other.Yaw;
	}

	Rotator operator+(const Rotator& Other)
	{
		Rotator New = Rotator(Pitch, Roll, Yaw);
		New.Pitch += Other.Pitch;
		New.Roll += Other.Roll;
		New.Yaw += Other.Yaw;

		return New;
	}

	void operator-=(const Rotator& Other)
	{
		Pitch -= Other.Pitch;
		Roll -= Other.Roll;
		Yaw -= Other.Yaw;
	}

	Rotator operator-(const Rotator& Other)
	{
		Rotator New = Rotator(Pitch, Roll, Yaw);
		New.Pitch -= Other.Pitch;
		New.Roll -= Other.Roll;
		New.Yaw -= Other.Yaw;

		return New;
	}

	void operator*=(const float& Other)
	{
		Pitch *= Other;
		Roll *= Other;
		Yaw *= Other;
	}

	Vector RotateVector(Vector Other)
	{
		Rotator Radians = Rotator::AsRadians(*this);
		DirectX::XMVECTOR UpdatedVector = DirectX::XMVector3Rotate(DirectX::XMVectorSet(Other.X, Other.Y, Other.Z, 0.0f), DirectX::XMQuaternionRotationRollPitchYaw(Radians.Pitch, Radians.Yaw, Radians.Roll));
		DirectX::XMFLOAT3 Rotated;
		DirectX::XMStoreFloat3(&Rotated, UpdatedVector);
		return Vector(Rotated.x, Rotated.y, Rotated.z);
	}

	Rotator RotateRotator(Rotator Other)
	{
		Rotator RotRadians = Rotator::AsRadians(*this);
		Rotator OtherRotRadians = Rotator::AsRadians(Other);

		DirectX::XMVECTOR ParentQuat = DirectX::XMQuaternionRotationRollPitchYaw(RotRadians.Pitch, RotRadians.Yaw, RotRadians.Roll);
		DirectX::XMVECTOR OtherQuat = DirectX::XMQuaternionRotationRollPitchYaw(OtherRotRadians.Pitch, OtherRotRadians.Yaw, OtherRotRadians.Roll);

		DirectX::XMVECTOR RotatedRot = DirectX::XMQuaternionMultiply(ParentQuat, OtherQuat);
		DirectX::XMFLOAT4 FinalRotation;
		DirectX::XMStoreFloat4(&FinalRotation, RotatedRot);

		//DirectX::XMFLOAT3 StoredRot;
		//DirectX::XMStoreFloat3(&StoredRot, RotatedRot);
		//Rotator NewRotation;

		//DirectX::XMQuaternionToAxisAngle(&DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), &NewRotation.Pitch, RotatedRot);
		//DirectX::XMQuaternionToAxisAngle(&DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), &NewRotation.Yaw, RotatedRot);
		//DirectX::XMQuaternionToAxisAngle(&DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), &NewRotation.Roll, RotatedRot);
		
		//TEST
		//DirectX::XMMATRIX RotMatrix = DirectX::XMMatrixRotationQuaternion(RotatedRot);
		//RotMatrix = DirectX::XMMatrixTranspose(RotMatrix);
		//DirectX::XMFLOAT4X4 RotMatrixTest;
		//DirectX::XMStoreFloat4x4(&RotMatrixTest, RotMatrix);

		//float fAngZ = atan2f(m[1], m[5]);
		//float fAngY = atan2f(m[8], m[10]);
		//float fAngX = -asinf(m[9]);

		//float fAngZ = atan2f(RotMatrixTest._11, RotMatrixTest._22);
		//float fAngY = atan2f(RotMatrixTest._42, RotMatrixTest._32);
		//float fAngX = -asinf(RotMatrixTest._31);

		//float fAngZ = atan2f(RotMatrixTest._32, RotMatrixTest._33);
		//float fAngY = atan2f(RotMatrixTest._21, RotMatrixTest._11);
		//float fAngX = atan2f(-RotMatrixTest._31, sqrt((RotMatrixTest._32 * RotMatrixTest._32) + (RotMatrixTest._33 * RotMatrixTest._33)));

		float t0 = 2.0f * (FinalRotation.w * FinalRotation.x + FinalRotation.y + FinalRotation.z);
		float t1 = 1.0f - 2.0f * (FinalRotation.x * FinalRotation.x + FinalRotation.y * FinalRotation.y);
		float t2 = 2.0f * (FinalRotation.w * FinalRotation.y - FinalRotation.z * FinalRotation.x);
		t2 = t2 > 1.0f ? 1.0f : t2;
		t2 = t2 < -1.0f ? -1.0f : t2;
		float t3 = 2.0f * (FinalRotation.w * FinalRotation.z + FinalRotation.x * FinalRotation.y);
		float t4 = 1.0f - 2.0f * (FinalRotation.y * FinalRotation.y + FinalRotation.z * FinalRotation.z);
		Rotator NewRotation = Rotator(asin(t2), atan2(t0, t1), atan2(t3, t4));


		
		FLOW_ENGINE_LOG("Translation: {0} to {1}", Other, Rotator::AsDegrees(NewRotation));

		return Rotator::AsDegrees(NewRotation);
	}

	static Rotator AsRadians(const Rotator& Rotation)
	{
		Rotator Rads;
		Rads.Pitch = (Rotation.Pitch * 3.14159f) / 180.0f;
		Rads.Roll = (Rotation.Roll * 3.14159f) / 180.0f;
		Rads.Yaw = (Rotation.Yaw * 3.14159f) / 180.0f;
		return Rads;
	}

	static Rotator AsDegrees(const Rotator& Rotation)
	{
		Rotator Rads;
		Rads.Pitch = (Rotation.Pitch * 180.0f) / 3.14159f;
		Rads.Roll = (Rotation.Roll * 180.0f) / 3.14159f;
		Rads.Yaw = (Rotation.Yaw * 180.0f) / 3.14159f;
		return Rads;
	}

	template<typename OStream>
	friend OStream& operator<<(OStream& os, const Rotator& c)
	{
		return os << c.Pitch << "," << c.Roll << "," << c.Yaw;
	}

	static float WrapAngle(float Theta)
	{
		const float modded = fmod(Theta, 360.0f);
		return (modded > 180) ?
			(modded - 360) :
			modded;
	}

private:


};
