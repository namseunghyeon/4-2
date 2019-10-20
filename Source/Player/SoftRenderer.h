#pragma once

#include "WindowsRSI.h"

class SoftRenderer
{
public:
	enum class RenderMode
	{
		TWO,
		THREE_PERSP
	};

	SoftRenderer(SoftRenderer const&) = delete;
	void operator=(SoftRenderer const&) = delete;
	static SoftRenderer& Inst()
	{
		static SoftRenderer instance;
		return instance;
	}

public:
	void SetRenderMode(RenderMode InRenderMode) { CurrentRenderMode = InRenderMode; }
	RenderMode GetRenderMode() const { return CurrentRenderMode; }
	void Initialize();
	void Shutdown();
	void PreUpdate();
	void Update();
	void PostUpdate();

	// fps
	float GetFrameFPS() const { return _frameFPS; }
	float GetAverageFPS() const { return _frameCount == 0 ? 0.0f : _frameCount / _elapsedTime; }
	float GetElapsedTime() const { return _elapsedTime; }
	int GetFrameCount() const { return _frameCount; }

private:
	SoftRenderer() { }
	~SoftRenderer() { Shutdown(); }

	RenderMode CurrentRenderMode = RenderMode::TWO;
	WindowsRSI* RSI = nullptr;

	UINT32 _screenSize;

	double _miliSecFreQuency = 0;
	double _frameMilliSec = 0;
	float _frameSec = 0;
	float _frameFPS = 0;
	float _elapsedTime = 0;
	INT64 _startFrameSec = 0;
	int _frameCount = 0;
};
