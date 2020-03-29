#pragma once

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
