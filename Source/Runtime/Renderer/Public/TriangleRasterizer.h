#pragma once

#include "Vector2.h"
#include "LinearColor.h"
#include "RenderingSoftwareInterface.h"

class TriangleRasterizer
{
public:
	TriangleRasterizer();
	~TriangleRasterizer();
	explicit TriangleRasterizer(Vertex InVertex0, Vertex InVertex1, Vertex InVertex2);

public:
	FORCEINLINE bool hasColor() const { return bHasColor; }
	FORCEINLINE bool hasUV() const { return bHasUV; }
	void recalcBounds();
	void generateContourBuffer(const ScreenPoint& InStartPos, const ScreenPoint& InEndPos);
	int* getContourBuffer() const { return contourBuffer; }
	int getContourBufferSize() const { return contourBufferSize; }


	FORCEINLINE Vector2 GetBaryCentricCoord(const Vector2& InScreenPosition) const
	{
		Vector2 w = InScreenPosition - Vector2(vertexBuffer[0].Position.X, vertexBuffer[0].Position.Y);
		float dotUW = uVector.Dot(w);
		float dotVW = vVector.Dot(w);
		float s = (VV * dotUW - UV * dotVW) * invDenom;
		float t = (UU * dotVW - UV * dotUW) * invDenom;
		return Vector2(s, t);
	}

	FORCEINLINE Vector2 GetUV(const Vector2& InScreenPosition) const
	{
		if (!bHasUV)
		{
			return Vector2::Zero;
		}
		Vector2 st = GetBaryCentricCoord(InScreenPosition);
		float oneMinusST = 1 - st.X - st.Y;
		return vertexBuffer[0].UV * oneMinusST + vertexBuffer[1].UV * st.X + vertexBuffer[2].UV * st.Y;
	}

	FORCEINLINE bool isInside(const Vector2& In) const
	{
		Vector2 st = getSt(In);
		if (st.X < 0.f) return false;
		if (st.Y < 0.f) return false;
		if ((st.X + st.Y) > 1.f) return false;

		return true;
	}

	FORCEINLINE LinearColor getColor(const Vector2& InScreenPosition) const
	{
		if (!hasColor())
		{
			return LinearColor::Error;
		}

		Vector2 st = getSt(InScreenPosition);
		float oneMinusST = 1 - st.X - st.Y;
		return vertexBuffer[0].Color * oneMinusST + vertexBuffer[1].Color * st.X + vertexBuffer[2].Color * st.Y;
	}

	FORCEINLINE Vector2 getSt(const Vector2& InScreenPosition) const
	{
		Vector2 w = InScreenPosition - Vector2(vertexBuffer[0].Position.X, vertexBuffer[0].Position.Y);
		float dotUW = uVector.Dot(w);
		float dotVW = vVector.Dot(w);
		float s = (VV * dotUW - UV * dotVW) * invDenom;
		float t = (UU * dotVW - UV * dotUW) * invDenom;
		return Vector2(s, t);
	}

public:
	Vertex vertexBuffer[3];
	ScreenPoint BottomRight;
	ScreenPoint TopLeft;

private:
	Vector2 uVector;
	Vector2 vVector;
	float UU, UV, VV, invDenom;

	int* contourBuffer = nullptr;
	int contourBufferSize = 0;

	bool bHasVertex = false;
	bool bHasColor = false;
	bool bHasUV = false;
};