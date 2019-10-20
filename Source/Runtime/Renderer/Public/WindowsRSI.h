
#pragma once

#include <windows.h>
#include "WindowsGDI.h"
#include "RenderingSoftwareInterface.h"

class WindowsRSI : public WindowsGDI, public RenderingSoftwareInterface
{
public:
	WindowsRSI() { };
	~WindowsRSI() { Shutdown(); }

public:
	virtual void Init(const bool InbSRGBColorSpace = true) override;
	virtual void Shutdown() override;

	virtual bool IsIntialized() const override { return bGDIInitialized; }
	virtual bool IsSRGBSpace() const override { return bSRGBColorSpace; }

	virtual void Clear(const LinearColor& InClearColor) override;
	virtual void BeginFrame() override;
	virtual void EndFrame() override { SwapBuffer(); }


	virtual void setVertexBuffer(Vertex* InVertexData) override;
	virtual void setIndexBuffer(const int* InIndexData) override;
	virtual void drawPrimitive(UINT InVertexSize, UINT InIndexSize) override;
	virtual void drawTopFlatTriangle(Vertex * tvs, bool DrawLastLine) override;
	virtual void drawBottomFlatTriangle(Vertex * tvs) override;
	virtual void drawLine(const Vector2& startVec,const Vector2& endvec, const LinearColor& inColor) override;

	virtual void DrawScreenPoint(const ScreenPoint& InScreenPoint, const LinearColor& inColor) override;

	virtual void DrawHorizontalLine(int InY, const LinearColor& InColor) override;
	virtual void DrawVerticalLine(int InX, const LinearColor& InColor) override;

private:
	Vertex* VertexBuffer;
	const int* IndexBuffer;
};