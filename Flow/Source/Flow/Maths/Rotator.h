#pragma once

//= Includes ===========================================

#include <DirectXMath.h>
#include "Vector3.h"

//= Class Definition ===================================

class FLOW_API Rotator
{
public:
	//= Constructors ===================================

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

	//= Operators ======================================

	void operator+=(const Rotator& Other)
	{
		Pitch += Other.Pitch;
		Roll += Other.Roll;
		Yaw += Other.Yaw;
	}

	Rotator operator+(const Rotator& Other) const
	{
		return Rotator(Pitch + Other.Pitch, Roll + Other.Roll, Yaw + Other.Yaw);
	}

	void operator-=(const Rotator& Other)
	{
		Pitch -= Other.Pitch;
		Roll -= Other.Roll;
		Yaw -= Other.Yaw;
	}

	Rotator operator-(const Rotator& Other) const
	{
		return Rotator(Pitch - Other.Pitch, Roll - Other.Roll, Yaw - Other.Yaw);
	}

	void operator*=(const float& Other)
	{
		Pitch *= Other;
		Roll *= Other;
		Yaw *= Other;
	}

	Rotator operator*(const float& Other) const
	{
		return Rotator(Pitch * Other, Roll * Other, Yaw * Other);
	}

	bool operator==(const Rotator& Other)
	{
		return 
			Pitch == Other.Pitch &&
			Roll == Other.Roll &&
			Yaw == Other.Yaw;
	}

	operator Vector3() const
	{
		return Vector3(Pitch, Roll, Yaw);
	}

	//Presumes the rotation is in degrees
	Vector3 GetForwardVector() const
	{
		Rotator Radians = Rotator::AsRadians(*this);
		DirectX::XMVECTOR UpdatedVector = DirectX::XMVector3Rotate(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), DirectX::XMQuaternionRotationRollPitchYaw(Radians.Pitch, Radians.Yaw, Radians.Roll));
		DirectX::XMFLOAT3 Rotated;
		DirectX::XMStoreFloat3(&Rotated, UpdatedVector);
		return Vector3(Rotated.x, Rotated.y, Rotated.z);
	}

	Vector3 RotateVector(Vector3 Other)
	{
		Rotator Radians = Rotator::AsRadians(*this);
		DirectX::XMVECTOR UpdatedVector = DirectX::XMVector3Rotate(DirectX::XMVectorSet(Other.x, Other.y, Other.z, 0.0f), DirectX::XMQuaternionRotationRollPitchYaw(Radians.Pitch, Radians.Yaw, Radians.Roll));
		DirectX::XMFLOAT3 Rotated;
		DirectX::XMStoreFloat3(&Rotated, UpdatedVector);
		return Vector3(Rotated.x, Rotated.y, Rotated.z);
	}

	//TODO: Need to figure out this logic
	Rotator RotateRotator(Rotator Other, bool IsRadians = false)
	{
		if (*this == Rotator(0.0f))
			return Other;

		Rotator RotRadians = IsRadians ? *this : Rotator::AsRadians(*this);
		Rotator OtherRotRadians = IsRadians ? Other : Rotator::AsRadians(Other);

		DirectX::XMVECTOR ParentQuat = DirectX::XMQuaternionRotationRollPitchYaw(RotRadians.Pitch, RotRadians.Yaw, RotRadians.Roll);
		DirectX::XMVECTOR OtherQuat = DirectX::XMQuaternionRotationRollPitchYaw(OtherRotRadians.Pitch, OtherRotRadians.Yaw, OtherRotRadians.Roll);

		DirectX::XMVECTOR RotatedRot = DirectX::XMQuaternionMultiply(OtherQuat, ParentQuat);
		DirectX::XMMATRIX Matrix = DirectX::XMMatrixRotationQuaternion(RotatedRot);

		DirectX::XMFLOAT4X4 EditMatrix;
		DirectX::XMStoreFloat4x4(&EditMatrix, Matrix);

		float fAngZ = atan2f(EditMatrix._12, EditMatrix._22);
		float fAngY = atan2f(EditMatrix._31, EditMatrix._33);
		float fAngX = -asinf(EditMatrix._32);

		Rotator NewRotation = Rotator(fAngX, fAngZ, fAngY);

		return Rotator::AsDegrees(NewRotation);
	}

	//= Static Functions ================================

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

	//= Public Variables ===================================

	float Pitch;
	float Roll;
	float Yaw;
};
