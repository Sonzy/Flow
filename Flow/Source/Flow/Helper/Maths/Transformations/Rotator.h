#pragma once

//TODO: Simplify wrapping, was lazy
struct Rotator
{
	float Pitch;
	float Roll;
	float Yaw;

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

private:

	float WrapAngle(float Theta)
	{
		const float modded = fmod(Theta, 360.0f);
		return (modded > 180) ?
			(modded - 360) :
			modded;
	}
};
