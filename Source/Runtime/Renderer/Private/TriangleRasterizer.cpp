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
	// screen bounds
	Vector2 sbbMin2D = Vector2(INFINITY, INFINITY);
	Vector2 sbbMax2D = Vector2(-INFINITY, -INFINITY);

	for (int i = 0; i < 3; i++)
	{
		if (vertexBuffer[i].Position.X < sbbMin2D.X) sbbMin2D.X = vertexBuffer[i].Position.X;
		if (vertexBuffer[i].Position.Y < sbbMin2D.Y) sbbMin2D.Y = vertexBuffer[i].Position.Y;
		if (vertexBuffer[i].Position.X > sbbMax2D.X) sbbMax2D.X = vertexBuffer[i].Position.X;
		if (vertexBuffer[i].Position.Y > sbbMax2D.Y) sbbMax2D.Y = vertexBuffer[i].Position.Y;
	}

	uVector = vertexBuffer[1].Position - vertexBuffer[0].Position;
	vVector = vertexBuffer[2].Position - vertexBuffer[0].Position;
	UU = uVector.Dot(uVector);
	UV = uVector.Dot(vVector);
	VV = vVector.Dot(vVector);
	invDenom = 1.f / (UU * VV - UV * UV);

	TopLeft = ScreenPoint(sbbMin2D);
	BottomRight = ScreenPoint(sbbMax2D);
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