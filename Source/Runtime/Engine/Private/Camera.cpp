#include "Precompiled.h"
#include "Camera.h"

Matrix4x4 Camera::GetTR() const
{
	float radRoll = Math::Deg2Rad(getRotate().Z);
	float radPitch = Math::Deg2Rad(getRotate().X);
	float radYaw = Math::Deg2Rad(getRotate().Y);

	float cR = cosf(radRoll);
	float cP = cosf(radPitch);
	float cY = cosf(radYaw);

	float sR = cosf(radRoll);
	float sP = cosf(radPitch);
	float sY = cosf(radYaw);

	Matrix4x4 tMat(Vector4::UnitY, Vector4::UnitY, Vector4::UnitZ, Vector4(getPosition().X, getPosition().Y, getPosition().Z, 1.f));
	Matrix4x4 rMat(
		Vector4(cR * cY + sR * sP * sY, -sR * cP, cR * sR - sR * sP * cY, 0.f),
		Vector4(cY * sR - cR * sP * sY, cR * cP, sR * sY + cR * sP *cP, 0.f),
		Vector4(-cP * sY, -sP, cP * cY, 0.f),
		Vector4(0.f, 0.f, 1.f, 0.f));

	//tMat[3] = -tMat[3];
	tMat[3][1] = -tMat[3][1];
	tMat[3][2] = -tMat[3][2];

	Matrix4x4 fMat = tMat * rMat;

	return fMat;
}
