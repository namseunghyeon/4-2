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

	// 2D Grid Options
	int Grid2DUnit = 10;
	ScreenPoint ScreenSize;

	// Input Manager
	InputManager InputManager;

	// Final Matrix
	Matrix4x4 FinalMatrix;
	float _fov = 60.f;

	//Camera camera = Camera(Vector3(0, -500, -500));
};
