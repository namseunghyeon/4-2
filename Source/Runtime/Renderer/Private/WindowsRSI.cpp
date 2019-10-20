
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

void WindowsRSI::drawTopFlatTriangle(Vertex * tvs, bool DrawLastLine = false)
{
	float dx1 = tvs[0].Position.X - tvs[2].Position.X;
	float dx2 = tvs[1].Position.X - tvs[2].Position.X;
	float dy = tvs[2].Position.Y - tvs[1].Position.Y;

	if (dy >= 0)
	{
		return;
	}

	float gradient1 = dx1 / dy;
	float gradient2 = dx2 / dy;

	PutPixel(ScreenPoint(tvs[2].Position), LinearColor(1.f, 0.f, 0.f));
	float startY = tvs[2].Position.Y;
	float startX = tvs[2].Position.X;
	float currentY = floorf(tvs[2].Position.Y) - 0.5f;
	float destY = tvs[1].Position.Y;
	while (currentY <= destY)
	{
		float deltaY = startY - currentY;
		float leftX = gradient1 * deltaY + startX;
		float rightX = gradient2 * deltaY + startX;
		int pixelX1 = Math::FloorToInt(leftX);
		int pixelX2 = Math::FloorToInt(rightX);
		int pixelY = Math::FloorToInt(currentY);
		for (int p = pixelX1; p <= pixelX2; ++p)
		{
			PutPixel(ScreenPoint(p, pixelY), LinearColor(1.f, 0.f, 0.f));
		}
		currentY += 1.0f;
	}

	if (DrawLastLine)
	{
		// 마지막 라인을 그린다.
		int pixelX1 = Math::FloorToInt(tvs[1].Position.X);
		int pixelX2 = Math::FloorToInt(tvs[2].Position.X);
		int pixelY = Math::FloorToInt(destY);
		for (int p = pixelX1; p <= pixelX2; ++p)
		{
			PutPixel(ScreenPoint(p, pixelY), LinearColor(1.f, 0.f, 0.f));
		}
	}
}

void WindowsRSI::drawBottomFlatTriangle(Vertex * tvs)
{
	float dx1 = tvs[1].Position.X - tvs[0].Position.X;
	float dx2 = tvs[2].Position.X - tvs[0].Position.X;
	float dy = tvs[0].Position.Y - tvs[1].Position.Y;

	if (dy <= 0)
	{
		return;
	}

	float gradient1 = dx1 / dy;
	float gradient2 = dx2 / dy;

	PutPixel(ScreenPoint(tvs[0].Position), LinearColor(0.f, 1.f, 0.f));
	float startY = tvs[0].Position.Y;
	float startX = tvs[0].Position.X;
	float currentY = floorf(tvs[0].Position.Y) - 0.5f;
	float destY = tvs[1].Position.Y;
	while (currentY >= destY)
	{
		float deltaY = startY - currentY;
		float leftX = gradient1 * deltaY + startX;
		float rightX = gradient2 * deltaY + startX;
		int startX = Math::FloorToInt(leftX);
		int endX = Math::FloorToInt(rightX);
		int pixelY = Math::FloorToInt(currentY);
		for (int p = startX; p <= endX; ++p)
		{
			PutPixel(ScreenPoint(p, pixelY), LinearColor(0.f, 1.f, 0.f));
		}
		currentY -= 1.0f;
	}

	// 마지막 라인을 그린다.
	int pixelX1 = Math::FloorToInt(tvs[0].Position.X);
	int pixelX2 = Math::FloorToInt(tvs[1].Position.X);
	int pixelY = Math::FloorToInt(destY);
	for (int p = pixelX1; p <= pixelX2; ++p)
	{
		PutPixel(ScreenPoint(p, pixelY), LinearColor(0.f, 1.f, 0.f));
	}
}

void WindowsRSI::drawPrimitive(UINT InVertexSize, UINT InIndexSize)
{
	if (VertexBuffer == nullptr || IndexBuffer == nullptr)
	{
		return;
	}

	UINT triangleCount = (int)(InIndexSize / 3);
	for (UINT ti = 0; ti < triangleCount; ti++)
	{
		Vertex tv[3] = {
			VertexBuffer[IndexBuffer[ti * 3]],
			VertexBuffer[IndexBuffer[ti * 3 + 1]],
			VertexBuffer[IndexBuffer[ti * 3 + 2]]
		};
		Vertex temp;

		// sort vertex position
		//ScreenPoint sortVertex[3]{ ScreenPoint(tv[0].Position),ScreenPoint(tv[1].Position),ScreenPoint(tv[2].Position) };
		//for (int i = 0, maxIdx = -1; i < 3 - 1; i++, maxIdx = -1)
		//{
		//	for (int j = i + 1; j < 3; j++)
		//	{
		//		if (sortVertex[i].Y < sortVertex[j].Y)
		//			maxIdx = j;

		//		else if (sortVertex[i].Y == sortVertex[j].Y && sortVertex[i].X > sortVertex[j].X)
		//			maxIdx = j;
		//	}

		//	if (maxIdx != -1)
		//	{
		//		ScreenPoint tmp = sortVertex[i];
		//		sortVertex[i] = sortVertex[maxIdx];
		//		sortVertex[maxIdx] = tmp;
		//	}
		//}

		if (tv[0].Position.Y == tv[1].Position.Y)
		{
			if (tv[0].Position.X > tv[1].Position.X)
			{
				temp = tv[0];
				tv[0] = tv[1];
				tv[1] = temp;
			}
		}
		else
		{
			if (tv[0].Position.Y < tv[1].Position.Y)
			{
				temp = tv[0];
				tv[0] = tv[1];
				tv[1] = temp;
			}
		}

		if (tv[1].Position.Y == tv[2].Position.Y)
		{
			if (tv[1].Position.X > tv[2].Position.X)
			{
				temp = tv[1];
				tv[1] = tv[2];
				tv[2] = temp;
			}
		}
		else
		{
			if (tv[1].Position.Y < tv[2].Position.Y)
			{
				temp = tv[1];
				tv[1] = tv[2];
				tv[2] = temp;
			}
		}


		if (tv[0].Position.Y == tv[1].Position.Y)
		{
			if (tv[0].Position.X > tv[1].Position.X)
			{
				temp = tv[0];
				tv[0] = tv[1];
				tv[1] = temp;
			}
		}
		else
		{
			if (tv[0].Position.Y < tv[1].Position.Y)
			{
				temp = tv[0];
				tv[0] = tv[1];
				tv[1] = temp;
			}
		}

		if (tv[0].Position.Y == tv[1].Position.Y)
		{
			drawTopFlatTriangle(tv);
		}
		else if (tv[1].Position.Y == tv[2].Position.Y)
		{
			drawBottomFlatTriangle(tv);
		}
		else
		{
			Vertex newV = tv[1];
			float height = tv[0].Position.Y - tv[2].Position.Y;
			float width = tv[2].Position.X - tv[0].Position.X;

			if (height == 0.0f)
			{
				return;
			}

			float gradient = width / height;
			newV.Position.X = gradient * (tv[0].Position.Y - tv[1].Position.Y) + tv[0].Position.X;

			if (newV.Position.X > tv[1].Position.X)
			{
				Vertex upperTriangle[3] = { tv[0], tv[1], newV };
				Vertex bottomTriangle[3] = { tv[1], newV, tv[2] };
				drawTopFlatTriangle(bottomTriangle);
				drawBottomFlatTriangle(upperTriangle);
			}
			else
			{
				Vertex upperTriangle[3] = { tv[0], newV, tv[1] };
				Vertex bottomTriangle[3] = { newV, tv[1], tv[2] };
				drawTopFlatTriangle(bottomTriangle);
				drawBottomFlatTriangle(upperTriangle);
			}
		}
	}
}

//void WindowsRSI::drawPrimitive(UINT InVertexSize, UINT InIndexSize)
//{
//	if (VertexBuffer == nullptr || IndexBuffer == nullptr)
//		return;
//
//	UINT triangleCount = (int)(InIndexSize / 3);
//	for (UINT ti = 0; ti < triangleCount; ti++)
//	{
//		TriangleRasterizer t(
//			VertexBuffer[IndexBuffer[ti * 3]],
//			VertexBuffer[IndexBuffer[ti * 3 + 1]],
//			VertexBuffer[IndexBuffer[ti * 3 + 2]]);
//
//		// sort vertex position
//		ScreenPoint sortVertex[3]{ ScreenPoint(t.vertexBuffer[0].Position),ScreenPoint(t.vertexBuffer[1].Position),ScreenPoint(t.vertexBuffer[2].Position) };
//		for (int i = 0, maxIdx = -1; i < 3 - 1; i++, maxIdx = -1)
//		{
//			for (int j = i + 1; j < 3; j++)
//			{
//				if (sortVertex[i].Y < sortVertex[j].Y)
//					maxIdx = j;
//
//				else if (sortVertex[i].Y <= sortVertex[j].Y && sortVertex[i].X > sortVertex[j].X)
//					maxIdx = j;
//			}
//
//			if (maxIdx != -1)
//			{
//				ScreenPoint tmp = sortVertex[i];
//				sortVertex[i] = sortVertex[maxIdx];
//				sortVertex[maxIdx] = tmp;
//			}
//		}
//
//		//ScreenPoint whVec[2]{ 
//		//	ScreenPoint(t.vertexBuffer[0].Position.X - t.vertexBuffer[1].Position.X, t.vertexBuffer[1].Position.Y - t.vertexBuffer[0].Position.Y),
//		//	ScreenPoint(t.vertexBuffer[0].Position.X - t.vertexBuffer[2].Position.X, t.vertexBuffer[1].Position.Y - t.vertexBuffer[2].Position.Y)};
//		// 왼쪽은 왼쪽대로  증가, 오른 족은 오른쪽 대로 증가,
//		// 가운데는 왼쪽과 오른 쪽으로 된 값에서 기울기를 구함.
//
//		/*int* contourBuffer = t.getContourBuffer();
//		int bufferSize = t.getContourBufferSize();*/
//
//		//for (int y = 0; y < bufferSize; ++y)
//		//{
//		//	int screenY = t.BottomRight.Y + y;
//		//	int startXValue = contourBuffer[y * 2 + 1];
//		//	int endXValue = contourBuffer[y * 2];
//
//		//	for (int screenX = startXValue; screenX <= endXValue; ++screenX)
//		//	{
//		//		ScreenPoint currentPixel(screenX, screenY);
//		//		SetPixelWithAlpha(currentPixel, t.getColor(currentPixel.ToVector2()));
//		//	}
//		//}
//	}
//}

void WindowsRSI::drawLine(const Vector2& startVec, const Vector2& endvec, const LinearColor& inColor)
{
	ScreenPoint curPos(startVec.X < endvec.X ? startVec : endvec);
	const ScreenPoint endPos(startVec.X < endvec.X ? endvec : startVec);
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