#pragma once

#include "CoreDefinition.h"
#include "GameObject2D.h"

class Camera2D : public GameObject2D
{
public:
	Matrix3x3 GetViewMatrix();
};