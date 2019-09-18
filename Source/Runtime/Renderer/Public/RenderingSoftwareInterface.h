
#pragma once

#include "DisplaySetting.h"
#include "LinearColor.h"

struct Vertex
{
	Vector2 Position;
	LinearColor Color = LinearColor::Error;
	Vector2 UV;
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

	virtual void SetVertexBuffer(Vertex* InVertexData) = 0;
	virtual void SetIndexBuffer(const int* InIndexData) = 0;
	virtual void DrawPrimitive(UINT InVertexSize, UINT InIndexSize) = 0;

	virtual void DrawScreenPoint(const ScreenPoint& InPoint, const LinearColor& inclearColor) = 0;
};
