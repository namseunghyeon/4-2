#include "Precompiled.h"
#include "Camera2D.h"

Matrix3x3 Camera2D::GetViewMatrix()
{
	return GetTransform().GetTRS();
}
