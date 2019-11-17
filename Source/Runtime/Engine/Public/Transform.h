#pragma once

class Transform
{
public:
	Transform(const Vector3& inPosition = Vector3::Zero, const Vector3& inRotate = Vector3::Zero, const Vector3& inScale = Vector3::One)
	{
		_positoin = inPosition;
		_rotate = inRotate;
		_scale = inScale;
	}

	Matrix4x4 GetTRS() const;
	void SetPosition(const Vector3& InV);
	void SetRotate(const float& InAngle);
	void SetRotate(const Vector3& InV);
	void SetScale(const Vector3& InV);
	void SetScale(const float& InV);

	void AddPosition(const Vector3& InV);
	void AddScale(const Vector3& InV);
	void AddRotate(const Vector3& InV);

	Vector3 getPosition() const { return _positoin; }
	Vector3 getRotate() const { return _rotate; }

	Vector3 getScale() const { return _scale; }

private:

	// member variable
	Vector3 _positoin;
	Vector3 _rotate;
	Vector3 _scale = Vector3(1,1,1);
};
