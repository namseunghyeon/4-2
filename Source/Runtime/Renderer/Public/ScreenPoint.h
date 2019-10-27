#pragma once

#include "CoreDefinition.h"
#include "Vector2.h"
#include "Vector3.h"

class DisplaySetting;

struct ScreenPoint
{
public:
	ScreenPoint() : X(0), Y(0) { }
	explicit ScreenPoint(int InX, int InY) : X(InX), Y(InY) { }
	//explicit ScreenPoint(float InX, float InY);
	explicit ScreenPoint(float InX, float InY) : X(Math::RoundToInt(InX)), Y(Math::RoundToInt(InY)) { }
	explicit ScreenPoint(const Vector2& InVec) : X(Math::RoundToInt(InVec.X)), Y(Math::RoundToInt(InVec.Y)) {};
	explicit ScreenPoint(const Vector3& InVec) : X(Math::RoundToInt(InVec.X)), Y(Math::RoundToInt(InVec.Y)) {};

	//explicit ScreenPoint(const Vector2& InVec) : X(Math::RoundToInt(InVec.X)), Y(Math::RoundToInt(InVec.Y)) {}

	FORCEINLINE ScreenPoint GetHalfSize() const
	{
		return ScreenPoint(Math::FloorToInt(X * 0.5f), Math::FloorToInt(Y * 0.5f));
	}

	FORCEINLINE Vector2 ToVector2() const
	{
		return Vector2((float)X + 0.5f, (float)Y + 0.5f);
	}

	int X;
	int Y;
};
