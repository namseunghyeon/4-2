#include "Precompiled.h"
#include "TriangleRasterizer.h"

TriangleRasterizer::TriangleRasterizer()
{
	recalcBounds();
}

TriangleRasterizer::~TriangleRasterizer()
{
	if (contourBuffer != nullptr)
	{
		delete[] contourBuffer;
		contourBuffer = nullptr;
	}
}

TriangleRasterizer::TriangleRasterizer(Vertex InVertex0, Vertex InVertex1, Vertex InVertex2)
{
	vertexBuffer[0] = InVertex0;
	vertexBuffer[1] = InVertex1;
	vertexBuffer[2] = InVertex2;

	bHasColor = true;
	bHasUV = true;

	recalcBounds();
}

void TriangleRasterizer::recalcBounds()
{
	Vector2 minPos = Vector2(INFINITY, INFINITY);
	Vector2 maxPos = Vector2(-INFINITY, -INFINITY);

	for (int i = 0; i < 3; i++)
	{
		if (vertexBuffer[i].Position.X < minPos.X) minPos.X = vertexBuffer[i].Position.X;
		if (vertexBuffer[i].Position.Y < minPos.Y) minPos.Y = vertexBuffer[i].Position.Y;
		if (vertexBuffer[i].Position.X > maxPos.X) maxPos.X = vertexBuffer[i].Position.X;
		if (vertexBuffer[i].Position.Y > maxPos.Y) maxPos.Y = vertexBuffer[i].Position.Y;
	}

	uVector = vertexBuffer[1].Position - vertexBuffer[0].Position;
	vVector = vertexBuffer[2].Position - vertexBuffer[0].Position;
	uU = uVector.Dot(uVector);
	uV = uVector.Dot(vVector);
	vV = vVector.Dot(vVector);
	invDenom = 1.f / (uU * vV - uV * uV);

	BottomRight = ScreenPoint(maxPos.X, minPos.Y);
	TopLeft = ScreenPoint(minPos.X, maxPos.Y);

	if (contourBuffer != nullptr)
	{
		delete[] contourBuffer;
	}

	contourBufferSize = TopLeft.Y - BottomRight.Y + 1;
	contourBuffer = new int[contourBufferSize * 2];

	for (int i = 0; i < contourBufferSize; i++)
	{
		contourBuffer[i * 2] = Math::IntMin;
	}

	for (int i = 0; i < contourBufferSize; i++)
	{
		contourBuffer[i * 2 + 1] = Math::IntMax;
	}

	generateContourBuffer(ScreenPoint(vertexBuffer[0].Position), ScreenPoint(vertexBuffer[1].Position));
	generateContourBuffer(ScreenPoint(vertexBuffer[0].Position), ScreenPoint(vertexBuffer[2].Position));
	generateContourBuffer(ScreenPoint(vertexBuffer[1].Position), ScreenPoint(vertexBuffer[2].Position));
}

void TriangleRasterizer::generateContourBuffer(const ScreenPoint& InStartPos, const ScreenPoint& InEndPos)
{
	int dx = Math::Abs(InEndPos.X - InStartPos.X);
	int sx = (InStartPos.X < InEndPos.X) ? 1 : -1;
	int dy = -Math::Abs(InEndPos.Y - InStartPos.Y);
	int sy = (InStartPos.Y < InEndPos.Y) ? 1 : -1;
	int err = dx + dy;

	int currentX = InStartPos.X;
	int currentY = InStartPos.Y;

	while (true)
	{
		int currentYIndex = currentY - BottomRight.Y;
		if (currentX > contourBuffer[currentYIndex * 2])
		{
			contourBuffer[currentYIndex * 2] = currentX;
		}

		if (currentX < contourBuffer[currentYIndex * 2 + 1])
		{
			contourBuffer[currentYIndex * 2 + 1] = currentX;
		}

		if (currentX == InEndPos.X && currentY == InEndPos.Y)
		{
			break;
		}

		int e2 = err * 2;
		if (e2 >= dy)
		{
			err += dy;
			currentX += sx;
		}

		if (e2 <= dx)
		{
			err += dx;
			currentY += sy;
		}
	}
}