
#include "Precompiled.h"
#include "WindowsRSI.h"
#include "TriangleRasterizer.h"

void WindowsRSI::Init(const bool InbSRGBColorSpace)
{
	InitializeGDI(DisplaySetting::Inst().GetSize(), InbSRGBColorSpace);
}

void WindowsRSI::Shutdown()
{
	ReleaseGDI();
}

void WindowsRSI::Clear(const LinearColor & InClearColor)
{
	SetColor(InClearColor);
	FillBuffer();

	ClearDepthBuffer();
}

void WindowsRSI::BeginFrame()
{
}

void WindowsRSI::DrawScreenPoint(const ScreenPoint & InScreenPoint, const LinearColor & inColor)
{
	PutPixel(InScreenPoint, inColor.ToColor32());
}

void WindowsRSI::SetVertexBuffer(Vertex* InVertexData)
{
	VertexBuffer = InVertexData;
}

void WindowsRSI::SetIndexBuffer(const int* InIndexData)
{
	IndexBuffer = InIndexData;
}

void WindowsRSI::DrawPrimitive(UINT InVertexSize, UINT InIndexSize)
{
	if (VertexBuffer == nullptr || IndexBuffer == nullptr)
	{
		return;
	}

	UINT triangleCount = (int)(InIndexSize / 3);
	for (UINT ti = 0; ti < triangleCount; ti++)
	{
		// Draw Each Triangle List 
		TriangleRasterizer t(
			VertexBuffer[IndexBuffer[ti * 3]],
			VertexBuffer[IndexBuffer[ti * 3 + 1]],
			VertexBuffer[IndexBuffer[ti * 3 + 2]]);

		//t.RecalcBounds();

		int* contourBuffer = t.GetContourBuffer();
		int bufferSize = t.GetContourBufferSize();

		for (int y = 0; y < bufferSize; ++y)
		{
			int screenY = t.BottomRight.Y + y;
			int startXValue = contourBuffer[y * 2 + 1];
			int endXValue = contourBuffer[y * 2];

			for (int screenX = startXValue; screenX <= endXValue; ++screenX)
			{
				ScreenPoint currentPixel(screenX, screenY);
				SetPixelWithAlpha(currentPixel, t.GetColor(currentPixel.ToVector2()));
			}
		}

	}
}
