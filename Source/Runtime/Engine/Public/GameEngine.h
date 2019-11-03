#pragma once

#include <string>
#include <map>
#include "Transform.h"
#include "GameObject.h"
#include "Camera.h"

class GameEngine
{
public:
	GameEngine() = default;

public:
	bool IsInitialized() const { return IsEngineInitialized; }
	void Init();
	void Shutdown();

private:
	bool IsEngineInitialized = false;
};

