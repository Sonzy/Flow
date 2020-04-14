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

	//TODO: Do operator that takes mod into account
	bool operator==(const Rotator& Other)
	{
		return Pitch == Other.Pitch &&
			Roll == Other.Roll &&
			Yaw == Other.Yaw;
	}

	Vector RotateVector(Vector Other)
	{
		Rotator Radians = Rotator::AsRadians(*this);
		DirectX::XMVECTOR UpdatedVector = DirectX::XMVector3Rotate(DirectX::XMVectorSet(Other.X, Other.Y, Other.Z, 0.0f), DirectX::XMQuaternionRotationRollPitchYaw(Radians.Pitch, Radians.Yaw, Radians.Roll));
		DirectX::XMFLOAT3 Rotated;
		DirectX::XMStoreFloat3(&Rotated, UpdatedVector);
		return Vector(Rotated.x, Rotated.y, Rotated.z);
	}

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

		FLOW_ENGINE_LOG("Rotating {0} by {1} to get {2}", Other, *this, Rotator::AsDegrees(NewRotation));
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
