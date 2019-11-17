#include "Precompiled.h"
#include "Camera.h"

Camera::Camera(const float & inFov)
{
	_fov = inFov;
}

Camera::Camera(const ScreenPoint& inScreenSize, const float &inFov)
{
	_fov = inFov;
	float repA = (float)inScreenSize.Y / (float)inScreenSize.X;
	float d = 1.f / tanf(Math::Deg2Rad(_fov) * 0.5f);
	float n = 5.5f;
	float f = 100.0f;

	float repNF = 1.f / (n - f);
	float k = f * repNF;
	float l = f * n * repNF;

	_pMat = Matrix4x4(
		Vector4::UnitX * repA * d,
		Vector4::UnitY * d,
		Vector4(0.f, 0.f, k, -1.f),
		Vector4(0.f, 0.f, l, 0.f));
}

Matrix4x4 Camera::GetTR() const
{
	Vector3 rotate = getRotate();
	Vector3 posiion = getPosition();

	float radRoll = Math::Deg2Rad(rotate.Z);
	float radPitch = Math::Deg2Rad(rotate.X);
	float radYaw = Math::Deg2Rad(rotate.Y);

	float cR = cosf(radRoll);
	float cP = cosf(radPitch);
	float cY = cosf(radYaw);

	float sR = cosf(radRoll);
	float sP = cosf(radPitch);
	float sY = cosf(radYaw);

	Matrix4x4 tMat(Vector4::UnitY, Vector4::UnitY, Vector4::UnitZ, Vector4(posiion.X, posiion.Y, posiion.Z, 1.f));
	Matrix4x4 rMat(
		Vector4(cR * cY + sR * sP * sY, -sR * cP, cR * sR - sR * sP * cY, 0.f),
		Vector4(cY * sR - cR * sP * sY, cR * cP, sR * sY + cR * sP *cP, 0.f),
		Vector4(-cP * sY, -sP, cP * cY, 0.f),
		Vector4(0.f, 0.f, 1.f, 0.f));

	tMat[3][1] = -tMat[3][1];
	tMat[3][2] = -tMat[3][2];

	Matrix4x4 fMat = tMat * rMat;
	
	return fMat;
}

void Camera::setPMat(const ScreenPoint& inScreenSize)
{
	float repA = (float)inScreenSize.Y / (float)inScreenSize.X;
	float d = 1.f / tanf(Math::Deg2Rad(_fov) * 0.5f);
	float n = 5.5f;
	float f = 100.0f;

	float repNF = 1.f / (n - f);
	float k = f * repNF;
	float l = f * n * repNF;

	_pMat = Matrix4x4(
		Vector4::UnitX * repA * d,
		Vector4::UnitY * d,
		Vector4(0.f, 0.f, k, -1.f),
		Vector4(0.f, 0.f, l, 0.f));
}

Matrix4x4 Camera::getLookMat(const Vector3& objectPos)
{
	Vector3 viewZ = (getPosition() - objectPos).Normalize();
	Vector3 viewX = Vector3::UnitY.Cross(viewZ).Normalize();
	if (viewX.IsZero())
		viewX = Vector3::UnitX;

	Vector3 viewY = viewZ.Cross(viewX).Normalize();
	Matrix4x4 virMat = Matrix4x4(Vector4(viewX, false), Vector4(viewY, false), Vector4(viewZ, false), Vector4::UnitW).Tranpose();
	Matrix4x4 vitMat = Matrix4x4(Vector4::UnitX, Vector4::UnitY, Vector4::UnitZ, Vector4(-getPosition()));
	Matrix4x4 vMat = virMat * vitMat;
	
	return  vMat;
}
