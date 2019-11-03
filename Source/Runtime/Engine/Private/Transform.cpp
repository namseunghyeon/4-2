#include "Precompiled.h"
#include "Transform.h"

Matrix4x4 Transfrom::GetTRS() const
{
	float radRoll = Math::Deg2Rad(_rotate.Z);
	float radPitch = Math::Deg2Rad(_rotate.X);
	float radYaw = Math::Deg2Rad(_rotate.Y);

	float cR = cosf(radRoll);
	float cP = cosf(radPitch);
	float cY = cosf(radYaw);

	float sR = cosf(radRoll);
	float sP = cosf(radPitch);
	float sY = cosf(radYaw);

	float cy, sy, cp, sp, cr, sr;
	Math::GetSinCos(cy, sy, radYaw);
	Math::GetSinCos(cp, sp, radPitch);
	Math::GetSinCos(cr, sr, radRoll);

	//Matrix4x4 tMat(Vector4::UnitX, Vector4::UnitY, Vector4::UnitZ, Vector4(cubePos));
	//Matrix4x4 sMat(Vector4::UnitX * cubeScaleXYZ, Vector4::UnitY * cubeScaleXYZ, Vector4::UnitZ * cubeScaleXYZ, Vector4::UnitW);
	//Matrix4x4 mMat = tMat * rMat * sMat;

	Matrix4x4 tMat(Vector4::UnitY, Vector4::UnitY, Vector4::UnitZ, Vector4(_positoin.X, _positoin.Y, _positoin.Z , 1.f));
	Matrix4x4 rMat(
		Vector4(cr * cy + sr * sp * sy, cy * sr - cr * sp * sy, -cp * sy, false),
		Vector4(-sr * cp, cr * cp, -sp, false),
		Vector4(cr * sy - sr * sp * cy, sr * sy + cr * sp * cy, cp * cy, false),
		Vector4::UnitW);
	Matrix4x4 sMat(Vector4::UnitY * _scale.X, Vector4::UnitY * _scale.Y, Vector4::UnitZ * _scale.Z, Vector4::UnitW);

	return tMat * rMat * sMat;
}

void Transfrom::SetPosition(Vector3 InV)
{
	_positoin = InV;
}

void Transfrom::SetRotate(float InAngle)
{
	_rotate.X = InAngle;
	_rotate.Y = InAngle;
	_rotate.Z = InAngle;
}

void Transfrom::SetRotate(Vector3 InV)
{
	_rotate = InV;
}

void Transfrom::SetScale(Vector3 InV)
{
	_scale = InV;
}

void Transfrom::AddPosition(const Vector3 & InV)
{
	_positoin += InV;
}

void Transfrom::AddScale(const Vector3 & InV)
{
	_scale += InV;
}

void Transfrom::AddRotate(const Vector3 & InV)
{
	_rotate += InV;
}
