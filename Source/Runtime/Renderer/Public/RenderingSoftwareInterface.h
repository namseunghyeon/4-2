
#pragma once

#include "DisplaySetting.h"
#include "LinearColor.h"

struct Vertex
{
	Vector2 Position;
	LinearColor Color = LinearColor::Error;
	Vector2 UV;
};

struct Slope
{
	int f;
	int d1;
	int d2;
	ScreenPoint pos;
	ScreenPoint whVec;
	Slope() = default;
	Slope(const int&inF, const int &inD1, const int &inD2, const ScreenPoint& InPos, const ScreenPoint InWH)
	{
		f = inF;
		d1 = inD1;
		d2 = inD2;
		pos = InPos;
		whVec = InWH;
	}
};

class RenderingSoftwareInterface
{
public:
	virtual void Init(const bool InbSRGBColorSpace = true) = 0;
	virtual void Shutdown() = 0;
	virtual bool IsIntialized() const = 0;
	virtual bool IsSRGBSpace() const = 0;

	virtual void Clear(const LinearColor& InClearColor) = 0;
	virtual void BeginFrame() = 0;
	virtual void EndFrame() = 0;

	virtual void setVertexBuffer(Vertex* InVertexData) = 0;
	virtual void setIndexBuffer(const int* InIndexData) = 0;
	virtual void drawPrimitive(UINT InVertexSize, UINT InIndexSize) = 0;
	virtual void drawBottomFlatTriangle(Vertex * tvs) = 0;
	virtual void drawTopFlatTriangle(Vertex * tvs, bool DrawLastLine) = 0;
	virtual void drawLine(const Vector2& startVec, const Vector2& endvec, const LinearColor& inColor) = 0;
	virtual void DrawScreenPoint(const ScreenPoint& InPoint, const LinearColor& inclearColor) = 0;

	virtual void DrawHorizontalLine(int InX, const LinearColor& InColor) = 0;
	virtual void DrawVerticalLine(int InY, const LinearColor& InColor) = 0;
};
