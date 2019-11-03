#pragma once

class Transfrom
{
public:
	FORCEINLINE Transfrom(const Vector3& InV) { _positoin = InV; }
	FORCEINLINE Transfrom() = default;

	Matrix4x4 GetTRS() const;
	void SetPosition(Vector3 InV);
	void SetRotate(float InAngle);
	void SetRotate(Vector3 InV);
	void SetScale(Vector3 InV);

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
	Vector3 _scale;
};
