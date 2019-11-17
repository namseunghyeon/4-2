#include "Precompiled.h"
#include "Transform.h"

Matrix4x4 Transform::GetTRS() const
{
	float cy, sy, cp, sp, cr, sr;
	Math::GetSinCos(cy, sy, _rotate.Y);
	Math::GetSinCos(cp, sp, _rotate.X);
	Math::GetSinCos(cr, sr, _rotate.Z);

	Matrix4x4 tMat(Vector4::UnitX, Vector4::UnitY, Vector4::UnitZ, Vector4(getPosition()));
	Matrix4x4 rMat(
		Vector4(cr * cy + sr * sp * sy, cy * sr - cr * sp * sy, -cp * sy, false),
		Vector4(-sr * cp, cr * cp, -sp, false),
		Vector4(cr * sy - sr * sp * cy, sr * sy + cr * sp * cy, cp * cy, false),
		Vector4::UnitW);

	Matrix4x4 sMat(Vector4::UnitX * _scale.X, Vector4::UnitY * _scale.Y, Vector4::UnitZ * _scale.Z, Vector4::UnitW);
	Matrix4x4 mMat = tMat * rMat * sMat;

	return tMat * rMat * sMat;
}

void Transform::SetPosition(const Vector3& InV)
{
	_positoin = InV;
}

void Transform::SetRotate(const float& InAngle)
{
	_rotate.X = InAngle;
	_rotate.Y = InAngle;
	_rotate.Z = InAngle;
}

void Transform::SetRotate(const Vector3& InV)
{
	_rotate = InV;
}

void Transform::SetScale(const Vector3& InV)
{
	_scale = InV;
}

void Transform::SetScale(const float & InV)
{
	_scale = Vector3::One * InV;
}

void Transform::AddPosition(const Vector3& InV)
{
	_positoin += InV;
}

void Transform::AddScale(const Vector3& InV)
{
	_scale += InV;
}

void Transform::AddRotate(const Vector3& InV)
{
	_rotate = _rotate + InV;
}
