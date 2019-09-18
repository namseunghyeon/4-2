#include "Precompiled.h"
#include "TriangleRasterizer.h"

TriangleRasterizer::TriangleRasterizer()
{
	RecalcBounds();
}

TriangleRasterizer::~TriangleRasterizer()
{
	if (ContourBuffer != nullptr)
	{
		delete[] ContourBuffer;
		ContourBuffer = nullptr;
	}
}

TriangleRasterizer::TriangleRasterizer(Vertex InVertex0, Vertex InVertex1, Vertex InVertex2)
{
	VertexBuffer[0] = InVertex0;
	VertexBuffer[1] = InVertex1;
	VertexBuffer[2] = InVertex2;

	bHasColor = true;
	bHasUV = true;

	RecalcBounds();
}

void TriangleRasterizer::RecalcBounds()
{
	Vector2 sbbMin2D = Vector2(INFINITY, INFINITY);
	Vector2 sbbMax2D = Vector2(-INFINITY, -INFINITY);

	for (int i = 0; i < 3; i++)
	{
		if (VertexBuffer[i].Position.X < sbbMin2D.X) sbbMin2D.X = VertexBuffer[i].Position.X;
		if (VertexBuffer[i].Position.Y < sbbMin2D.Y) sbbMin2D.Y = VertexBuffer[i].Position.Y;
		if (VertexBuffer[i].Position.X > sbbMax2D.X) sbbMax2D.X = VertexBuffer[i].Position.X;
		if (VertexBuffer[i].Position.Y > sbbMax2D.Y) sbbMax2D.Y = VertexBuffer[i].Position.Y;
	}

	UVector = VertexBuffer[1].Position - VertexBuffer[0].Position;
	VVector = VertexBuffer[2].Position - VertexBuffer[0].Position;
	DotUU = UVector.Dot(UVector);
	DotUV = UVector.Dot(VVector);
	DotVV = VVector.Dot(VVector);
	InvDenom = 1.f / (DotUU * DotVV - DotUV * DotUV);

	BottomRight = ScreenPoint(sbbMax2D.X, sbbMin2D.Y);
	TopLeft = ScreenPoint(sbbMin2D.X, sbbMax2D.Y);

	if (ContourBuffer != nullptr)
	{
		delete[] ContourBuffer;
	}

	ContourBufferSize = TopLeft.Y - BottomRight.Y + 1;
	ContourBuffer = new int[ContourBufferSize * 2];

	for (int i = 0; i < ContourBufferSize; i++)
	{
		ContourBuffer[i * 2] = Math::IntMin;
	}

	for (int i = 0; i < ContourBufferSize; i++)
	{
		ContourBuffer[i * 2 + 1] = Math::IntMax;
	}

	GenerateContourBuffer(ScreenPoint(VertexBuffer[0].Position), ScreenPoint(VertexBuffer[1].Position));
	GenerateContourBuffer(ScreenPoint(VertexBuffer[0].Position), ScreenPoint(VertexBuffer[2].Position));
	GenerateContourBuffer(ScreenPoint(VertexBuffer[1].Position), ScreenPoint(VertexBuffer[2].Position));

}

void TriangleRasterizer::GenerateContourBuffer(const ScreenPoint& InStartPos, const ScreenPoint& InEndPos)
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
		if (currentX > ContourBuffer[currentYIndex * 2])
		{
			ContourBuffer[currentYIndex * 2] = currentX;
		}

		if (currentX < ContourBuffer[currentYIndex * 2 + 1])
		{
			ContourBuffer[currentYIndex * 2 + 1] = currentX;
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
