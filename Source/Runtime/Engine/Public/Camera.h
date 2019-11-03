#pragma once

#include "GameObject.h"

class Camera : public GameObject
{
public:
	FORCEINLINE Camera(const Vector3& InV) { SetPosition(InV); }
	FORCEINLINE Camera() = default;

	Matrix4x4 GetTR() const;
};