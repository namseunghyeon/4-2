#pragma once
#include "RenderingSoftwareInterface.h"
#include "InputManager.h"

class SoftRendererImpl3D
{
public:
	SoftRendererImpl3D(class SoftRenderer* InOwner);
	~SoftRendererImpl3D();

public:
	void RenderFrameImpl();
	void UpdateImpl(float DeltaSeconds);

private:
	RenderingSoftwareInterface* RSI = nullptr;

	int Grid2DUnit = 10;
	ScreenPoint ScreenSize;

	InputManager InputManager;

	Matrix4x4 FinalMatrix;

	float _fov;
};
