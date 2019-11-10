#pragma once

class Camera : public GameObject
{
public:
	Camera(const ScreenPoint& inScreenSize, const float& inFov = 60.f);

	Matrix4x4 GetTR() const;

	void setPMat(const ScreenPoint& inScreenSize, const float& inFov = 60.f);
	Matrix4x4 getPMat() const { return _pMat; }

private:
	float _fov;

	Matrix4x4 _pMat;
};