
#include "WindowsPrecompiled.h"
#include "SoftRenderer.h"
#include "WindowsRSI.h"

void SoftRenderer::Initialize()
{
	RSI = new WindowsRSI();
	if (RSI != nullptr)
	{
		RSI->Init(false);
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

		// Render Code Begin
		RSI->DrawScreenPoint(ScreenPoint(0, 0), LinearColor(1.f, 0.f, 0.f));

		Vector2 center(0.f, 0.f);
		float radius = 100.f;

		Vector2 minPos(center.X - radius, center.Y - radius);
		Vector2 maxPos(center.X + radius, center.Y + radius);

		ScreenPoint minPoint(minPos);
		ScreenPoint maxPoint(maxPos);

		LinearColor circleColor(1.f, 0.f, 0.f, 1.f);
		for (int x = minPoint.X; x <= maxPoint.X; x++)
		{
			for (int y = minPoint.Y; y <= maxPoint.Y; y++)
			{
				ScreenPoint targetPixel(x, y);
				Vector2 targetPos = targetPixel.ToVector2();
				float disSquare = (targetPos - center).SizeSquared();
				if (disSquare < (radius * radius))
				{
					RSI->DrawScreenPoint(ScreenPoint(x, y), circleColor);
				}
			}
		}

		// rende Code End
		RSI->EndFrame();
	}
}


