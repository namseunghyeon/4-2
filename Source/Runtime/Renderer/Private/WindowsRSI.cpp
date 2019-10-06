
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

void WindowsRSI::Clear(const LinearColor& InClearColor)
{
	SetColor(InClearColor);
	FillBuffer();

	ClearDepthBuffer();
}

void WindowsRSI::BeginFrame()
{
}

void WindowsRSI::DrawScreenPoint(const ScreenPoint& InScreenPoint, const LinearColor& inColor)
{
	PutPixel(InScreenPoint, inColor.ToColor32());
}

void WindowsRSI::DrawHorizontalLine(int InY, const LinearColor& InColor)
{
	Color32 color = InColor.ToColor32();
	Color32* dest = ScreenBuffer;
	int startIndex = Math::FloorToInt(((float)ScreenSize.Y - 1.f) * 0.5f) - InY;
	dest = dest + startIndex * ScreenSize.X;
	for (int x = 0; x < ScreenSize.X; x++)
	{
		*dest = color;
		dest++;
	}
}

void WindowsRSI::DrawVerticalLine(int InX, const LinearColor& InColor)
{
	Color32 color = InColor.ToColor32();
	Color32* dest = ScreenBuffer;

	int xIndex = InX + Math::FloorToInt(((float)ScreenSize.X - 1.f) * 0.5f);
	if (xIndex < 0) return;
	for (int y = 0; y < ScreenSize.Y; ++y)
	{
		*(dest + xIndex) = color;
		dest += ScreenSize.X;
	}
}

void WindowsRSI::setVertexBuffer(Vertex* InVertexData)
{
	VertexBuffer = InVertexData;
}

void WindowsRSI::setIndexBuffer(const int* InIndexData)
{
	IndexBuffer = InIndexData;
}

void WindowsRSI::drawPrimitive(UINT InVertexSize, UINT InIndexSize)
{
	if (VertexBuffer == nullptr || IndexBuffer == nullptr)
		return;

	UINT triangleCount = (int)(InIndexSize / 3);
	for (UINT ti = 0; ti < triangleCount; ti++)
	{
		TriangleRasterizer t(
			VertexBuffer[IndexBuffer[ti * 3]],
			VertexBuffer[IndexBuffer[ti * 3 + 1]],
			VertexBuffer[IndexBuffer[ti * 3 + 2]]);

		int* contourBuffer = t.getContourBuffer();
		int bufferSize = t.getContourBufferSize();

		for (int y = 0; y < bufferSize; ++y)
		{
			int screenY = t.BottomRight.Y + y;
			int startXValue = contourBuffer[y * 2 + 1];
			int endXValue = contourBuffer[y * 2];

			for (int screenX = startXValue; screenX <= endXValue; ++screenX)
			{
				ScreenPoint currentPixel(screenX, screenY);
				SetPixelWithAlpha(currentPixel, t.getColor(currentPixel.ToVector2()));
			}
		}

	}
}

void WindowsRSI::drawLine(const Vector2& startVec, const Vector2& endvec, const LinearColor& inColor)
{
	ScreenPoint curPos = ScreenPoint(startVec.X < endvec.X ? startVec : endvec);
	const ScreenPoint endPos = ScreenPoint(startVec.X < endvec.X ? endvec : startVec);
	ScreenPoint whVec(endPos.X - startVec.X, endPos.Y - startVec.Y);

	int f, d1, d2;

	Color32 curColor = inColor.ToColor32();
	// x = w, y = h
	if (whVec.Y > 0)
	{
		whVec.Y = Math::Abs(whVec.Y);
		if (whVec.X > whVec.Y)
		{
			f = 2 * whVec.Y - whVec.X;
			d1 = 2 * whVec.Y;
			d2 = 2 * (whVec.Y - whVec.X);

			for (; curPos.X <= endPos.X; curPos.X++)
			{
				PutPixel(curPos, curColor);

				if (f < 0)
				{
					f += d1;
				}
				else
				{
					++curPos.Y;
					f +=d2;
				}
			}
		}
		else
		{
			f = whVec.Y - 2 * whVec.X;
			d1 = -2 * whVec.X;
			d2 = 2 * (whVec.Y - whVec.X);

			for (; curPos.Y <= endPos.Y; curPos.Y++)
			{
				PutPixel(curPos, curColor);

				if (f >= 0)
				{
					f += d1;
				}
				else
				{
					++curPos.X;
					f += d2;
				}
			}
		}
	}
	else
	{
		whVec.Y = Math::Abs(whVec.Y);
		if (whVec.X > whVec.Y)
		{
			f = 2 * whVec.Y - whVec.X;
			d1 = 2 * whVec.Y;
			d2 = 2 * (whVec.Y - whVec.X);

			for (; curPos.X <= endPos.X; curPos.X++)
			{
				PutPixel(curPos, curColor);

				if (f < 0)
				{
					f += d1;
				}
				else
				{
					--curPos.Y;
					f += d2;
				}
			}
		}
		else
		{
			f = whVec.Y - 2 * whVec.X;
			d1 = -2 * whVec.X;
			d2 = 2 * (whVec.Y - whVec.X);

			for (; curPos.Y > endPos.Y; curPos.Y--)
			{
				PutPixel(curPos, curColor);

				if (f >= 0)
				{
					f += d1;
				}
				else
				{
					++curPos.X;
					f += d2;
				}
			}
		}
	}
}