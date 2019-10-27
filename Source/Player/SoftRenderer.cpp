
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

		_mainTex.LoadPNGFile("Steve.png");
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

		static float moveSpeed = 50.f;
		Vector2 addPos(Vector2(_inputManager.GetXAxis() * moveSpeed, _inputManager.GetYAxis() * moveSpeed) * _frameSec);
		static Camera2D camera2D;
		camera2D.GetTransform().AddPosition(addPos);

		static float angle = 1;
		static float rotateSpeed = 5.0f;
		static GameObject2D quad;
		static Vector2 quadSize(10.f, 10.f);
		static Vector2 quadPos(10.f, 10.f);
		static float min = 0.8f, max = 1.6f;
		float sinWave = sinf(_elapsedTime * Math::TwoPI) * (max - min);
		//quad.GetTransform().AddRotation(angle);
		quad.GetTransform().SetScale(quadSize);
		//quad.GetTransform().SetScale(quadSize * sinWave);
		Matrix3x3 fMat = camera2D.GetViewMatrix() * quad.GetTransform().GetTRS();

		// Define Mesh Data
		Vertex v[4];
		v[0].Position = Vector2(-10.0f, -10.0f);
		v[0].Color = LinearColor(1.0f, 0.0f, 0.0f);
		v[1].Position = Vector2(-10.0f, 10.0f);
		v[1].Color = LinearColor(1.0f, 0.0f, 0.0f);
		v[2].Position = Vector2(10.0f, 10.0f);
		v[2].Color = LinearColor(0.0f, 0.0f, 1.0f);
		v[3].Position = Vector2(10.0f, -10.0f);
		v[3].Color = LinearColor(0.0f, 1.0f, 0.0f);

		v[0].UV = Vector2(0.25f, 0.25f);
		v[1].UV = Vector2(0.25f, 0.125f);
		v[2].UV = Vector2(0.125f, 0.125f);
		v[3].UV = Vector2(0.125f, 0.25f);

		for (int i = 0; i < 4; i++)
		{
			v[i].Position = fMat * v[i].Position;
		}

		int i[6];
		i[0] = 0;
		i[1] = 1;
		i[2] = 2;
		i[3] = 0;
		i[4] = 2;
		i[5] = 3;

		// Draw Call
		RSI->SetTexture(RSITexture(_mainTex.GetBuffer(), _mainTex.GetWidth(), _mainTex.GetHeight()));
		RSI->setVertexBuffer(v);
		RSI->setIndexBuffer(i);
		//RSI->drawPrimitive(4, 6);
		RSI->oldDrawPrimitive(4, 6);

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


