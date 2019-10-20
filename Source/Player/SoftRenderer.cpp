
#include "WindowsPrecompiled.h"
#include "SoftRenderer.h"
#include "WindowsRSI.h"

void SoftRenderer::Initialize()
{
	RSI = new WindowsRSI();
	if (RSI != nullptr)
	{
		RSI->Init(false);

		// fps
		LARGE_INTEGER frequency;
		if (!QueryPerformanceFrequency(&frequency))
		{
			Shutdown();
			return;
		}

		_miliSecFreQuency = frequency.QuadPart / 1000;

		_frameCount = 0;
		_elapsedTime = 0;

		_screenSize = DisplaySetting::Inst().GetSize().X * DisplaySetting::Inst().GetSize().Y;
	}
}

void SoftRenderer::Shutdown()
{
	if (RSI != nullptr && RSI->IsIntialized())
	{
		RSI->Shutdown();
		delete RSI;
		RSI = nullptr;
	}
}

void SoftRenderer::PreUpdate()
{
	if (RSI == nullptr) return;

	static LARGE_INTEGER preCycles;
	QueryPerformanceCounter(&preCycles);
	_startFrameSec = preCycles.QuadPart;
}

void SoftRenderer::Update()
{
	if (RSI != nullptr)
	{
		// RSI->BeginFrame();
		RSI->Clear(LinearColor(0.125f, 0.5f, 1.f, 1.f));

		// Axis Line
		RSI->DrawHorizontalLine(0, LinearColor(1.f, 0.f, 0.f, 1.f));
		RSI->DrawVerticalLine(0, LinearColor(0.f, 1.f, 0.f, 1.f));

		// Grid Line
		static ScreenPoint screenSize = DisplaySetting::Inst().GetSize();

		static int gridSize = 10;
		static int halfX = Math::FloorToInt(((float)screenSize.X - 1.f) * 0.5f);
		static int halfY = Math::FloorToInt(((float)screenSize.Y - 1.f) * 0.5f);

		for (int x = gridSize; x < halfX; x += gridSize)
		{
			RSI->DrawVerticalLine(x, LinearColor(0.5f, 0.5f, 0.5f, 1.f));
			RSI->DrawVerticalLine(-x, LinearColor(0.5f, 0.5f, 0.5f, 1.f));
		}

		for (int y = gridSize; y < halfY; y += gridSize)
		{
			RSI->DrawHorizontalLine(y, LinearColor(0.5f, 0.5f, 0.5f, 1.f));
			RSI->DrawHorizontalLine(-y, LinearColor(0.5f, 0.5f, 0.5f, 1.f));
		}

		float cos = 0.0f, sin = 0.f;
		static float angle = 0,f;
		static float rotSpeed = 180.0f;
		angle += (_frameSec) * rotSpeed;
		Math::GetSinCos(sin,cos, angle);
		static float min = 0.8f, max = 1.6f;
		float sinWave = sinf(_elapsedTime * Math::TwoPI) * min;

		Matrix2x2 scale(Vector2::UnitX * sinWave, Vector2::UnitY * sinWave);
		Matrix2x2 rotate(Vector2(cos, sin), Vector2(-sin, cos));
		Matrix2x2 rs = rotate * scale;

		// Define Mesh Data
		Vertex v[9];
		v[0].Position = Vector2(-150.0f, -100.0f);
		v[0].Color = LinearColor(1.0f, 0.0f, 0.0f);
		v[1].Position = Vector2(-200.0f, 100.0f);
		v[1].Color = LinearColor(0.0f, 1.0f, 0.0f);
		v[2].Position = Vector2(-100.0f, 100.0f);
		v[2].Color = LinearColor(0.0f, 0.0f, 1.0f);

		v[3].Position = Vector2(150.0f, 100.0f);
		v[3].Color = LinearColor(1.0f, 0.0f, 0.0f);
		v[4].Position = Vector2(100.0f, -100.0f);
		v[4].Color = LinearColor(0.0f, 1.0f, 0.0f);
		v[5].Position = Vector2(200.0f, -100.0f);
		v[5].Color = LinearColor(0.0f, 0.0f, 1.0f);

		v[6].Position = Vector2(50.0f, 100.0f);
		v[6].Color = LinearColor(1.0f, 0.0f, 0.0f);
		v[7].Position = Vector2(-50.0f, 0.0f);
		v[7].Color = LinearColor(0.0f, 1.0f, 0.0f);
		v[8].Position = Vector2(0.0f, -100.0f);
		v[8].Color = LinearColor(0.0f, 0.0f, 1.0f);

		for (int i = 0; i < 9; i++)
		{
			v[i].Position = rs * v[i].Position;
		}

		int i[9];
		i[0] = 0;
		i[1] = 1;
		i[2] = 2;
		i[3] = 3;
		i[4] = 4;
		i[5] = 5;
		i[6] = 6;
		i[7] = 7;
		i[8] = 8;

		// Draw Call
		RSI->setVertexBuffer(v);
		RSI->setIndexBuffer(i);
		RSI->drawPrimitive(9, 9);

		// rende Code End
		RSI->EndFrame();
	}
}

void SoftRenderer::PostUpdate()
{
	if (RSI == nullptr) return;

	_frameCount++;
	static LARGE_INTEGER postCycles;
	QueryPerformanceCounter(&postCycles);
	_frameMilliSec = double(postCycles.QuadPart - _startFrameSec) / _miliSecFreQuency;
	_frameSec = (float)(_frameMilliSec / 1000);
	_elapsedTime += _frameSec;
	_frameFPS = _frameSec == 0 ? 0 : 1 / _frameSec;
}


