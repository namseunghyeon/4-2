#pragma once

#define defaultFov 60.f

class Camera : public GameObject
{
public:
	Camera(const float& inFov = defaultFov);
	Camera(const ScreenPoint& inScreenSize, const float& inFov = defaultFov);

	void setPMat(const ScreenPoint& inScreenSize);
	Matrix4x4 getPMat() const { return _pMat; }

	void setFov(const float& inFov) { _fov = inFov; }
	float getFov() const { return _fov; }

	Matrix4x4 GetTR() const;
	Matrix4x4 getLookMat(const Vector3& objectPos);

private:
	float _fov;

	Matrix4x4 _pMat;
};