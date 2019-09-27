
#include "WindowsPrecompiled.h"
#include "SoftRenderer.h"
#include "WindowsRSI.h"

void SoftRenderer::Initialize()
{
	RSI = new WindowsRSI();
	if (RSI != nullptr)
	{
		RSI->Init(false);

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
		ScreenPoint screenSize = DisplaySetting::Inst().GetSize();


		int gridSize = 10;
		int halfX = Math::FloorToInt(((float)screenSize.X - 1.f) * 0.5f);
		int halfY = Math::FloorToInt(((float)screenSize.Y - 1.f) * 0.5f);

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

		Vertex vertices[4];
		vertices[0].Position = Vector2(-100, -100.f);
		vertices[0].Color = LinearColor(1.f, 0.f, 0.f);

		vertices[1].Position = Vector2(-100.f, 100.f);
		vertices[1].Color = LinearColor(0.f, 1.f, 0.f);

		vertices[2].Position = Vector2(100.f, 100.f);
		vertices[2].Color = LinearColor(0.f, 0.f, 1.f);

		vertices[3].Position = Vector2(100.f, -100.f);
		vertices[3].Color = LinearColor(1.f, 0.f, 0.f);

		int indices[6]{ 0,1,2,0,2,3 };

		// Draw Call
		RSI->setVertexBuffer(vertices);
		RSI->setIndexBuffer(indices);
		RSI->drawPrimitive(4, 6);

		// rende Code End
		RSI->EndFrame();
	}
}


