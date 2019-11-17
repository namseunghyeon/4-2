
#include "Precompiled.h"
#include "WindowsRSI.h"
#include "WindowsGDI.h"

WindowsRSI::~WindowsRSI()
{
}

bool WindowsRSI::Init(const ScreenPoint& InScreenSize)
{
	SetBlendingMode(BlendingMode);
	return InitializeGDI(InScreenSize);
}

void WindowsRSI::Shutdown()
{
	ReleaseGDI();
}

void WindowsRSI::SetBlendingMode(BlendingModes InNewBlendingMode)
{
	BlendingMode = InNewBlendingMode;
	//using namespace std::placeholders;
	//if (BlendingMode == BlendingModes::Opaque)
	//{
	//	SetPixel = std::bind(&WindowsGDI::SetPixelOpaque, this, _1, _2);
	//}
	//else if (BlendingMode == BlendingModes::AlphaBlending)
	//{
	//	SetPixel = std::bind(&WindowsGDI::SetPixelAlphaBlending, this, _1, _2);
	//}
}

void WindowsRSI::Clear(const LinearColor & InClearColor)
{
	FillBuffer(InClearColor.ToColor32());
	ClearDepthBuffer();
}

void WindowsRSI::BeginFrame()
{
}

void WindowsRSI::EndFrame()
{
	SwapBuffer();
}

void WindowsRSI::DrawPoint(const Vector2& InVectorPos, const LinearColor& InColor)
{
	SetPixel(ScreenPoint::ToScreenCoordinate(ScreenSize, InVectorPos), InColor);
}

void WindowsRSI::DrawLine(const Vector2 & InStartPos, const Vector2 & InEndPos, const LinearColor & InColor)
{
	ScreenPoint startPos = ScreenPoint::ToScreenCoordinate(ScreenSize, InStartPos);
	ScreenPoint endPos = ScreenPoint::ToScreenCoordinate(ScreenSize, InEndPos);

	int w = endPos.X - startPos.X;
	int h = endPos.Y - startPos.Y;

	bool slope = (Math::Abs(w) >= Math::Abs(h));
	int dirx = w > 0 ? 1 : -1;
	int diry = h > 0 ? 1 : -1;
	int fw = dirx * w;
	int fh = diry * h;

	int f = slope ? fh * 2 - fw : 2 * fw - fh;
	int f1 = slope ? 2 * fh : 2 * fw;
	int f2 = slope ? 2 * (fh - fw) : 2 * (fw - fh);
	int x = startPos.X;
	int y = startPos.Y;

	if (slope)
	{
		while (x != endPos.X)
		{
			SetPixel(ScreenPoint(x, y), InColor);

			if (f < 0)
			{
				f += f1;
			}
			else
			{
				f += f2;
				y += diry;
			}

			x += dirx;
		}
	}
	else
	{
		while (y != endPos.Y)
		{
			SetPixel(ScreenPoint(x, y), InColor);

			if (f < 0)
			{
				f += f1;
			}
			else
			{
				f += f2;
				x += dirx;
			}

			y += diry;
		}
	}
}

void WindowsRSI::DrawFullVerticalLine(int InX, const LinearColor & InColor)
{
	if (InX < 0 || InX >= ScreenSize.X)
	{
		return;
	}

	for (int y = 0; y < ScreenSize.Y; ++y)
	{
		SetPixel(ScreenPoint(InX, y), InColor);
	}

	return;
}

void WindowsRSI::DrawFullHorizontalLine(int InY, const LinearColor & InColor)
{
	if (InY < 0 || InY >= ScreenSize.Y)
	{
		return;
	}

	for (int x = 0; x < ScreenSize.X; ++x)
	{
		SetPixel(ScreenPoint(x, InY), InColor);
	}
}

void WindowsRSI::SetUniformMatrix(Matrix4x4 * InMatrixData)
{
	// 0 == camera TR
	// 1 == camPmat P
	// 2 == m Mat.
	_uniformMat = InMatrixData;

}

//void WindowsRSI::SetVertexBuffer(VertexData * InVertexData)
//{
//	_vertexData = InVertexData;
//}
//
//void WindowsRSI::SetIndexBuffer(int * InIndexData)
//{
//	_indexData = InIndexData;
//}

//void WindowsRSI::DrawPrimitive(UINT InVertexSize, UINT InIndexSize)
//{
//	static float repA = (float)ScreenSize.Y / (float)ScreenSize.X;
//	//static float d = 1.f / tanf(Math::Deg2Rad(_fov) * 0.5f);
//	static float d = 1.f / tanf(Math::Deg2Rad(60) * 0.5f);
//	Matrix4x4 finMat = _uniformMat[0] * _uniformMat[1] * _uniformMat[2];
//	int triangleCount = InIndexSize / 3;
//	for (int t = 0; t < triangleCount; t++)
//	{
//		Vector4 tp[3];
//		tp[0] = _vertexData[_indexData[t * 3]].Position;
//		tp[1] = _vertexData[_indexData[t * 3 + 1]].Position;
//		tp[2] = _vertexData[_indexData[t * 3 + 2]].Position;
//
//		for (int ti = 0; ti < 3; ti++)
//		{
//			tp[ti] = finMat * tp[ti];
//			float repW = 1.f / tp[ti].W;
//			tp[ti].X *= repW;
//			tp[ti].Y *= repW;
//			tp[ti].Z *= repW;
//		}
//
//		Vector3 edge1 = (tp[1] - tp[0]).ToVector3();
//		Vector3 edge2 = (tp[2] - tp[0]).ToVector3();
//		Vector3 faceNormal = edge2.Cross(edge1).Normalize();
//		static Vector3 cameraDir = -Vector3::UnitZ;
//		if (cameraDir.Dot(faceNormal) < 0.f)
//			continue;
//
//		for (int ti = 0; ti < 3; ti++)
//		{
//			float repZ = 1.f / -tp[ti].Z;
//			tp[ti].Y = tp[ti].Y * d * repZ;
//			tp[ti].X = tp[ti].X * d * repZ * repA;
//
//			tp[ti].X *= (ScreenSize.X * 0.5f);
//			tp[ti].Y *= (ScreenSize.Y * 0.5f);
//		}
//
//		DrawLine(tp[0].ToVector2(), tp[1].ToVector2(), LinearColor::Red);
//		DrawLine(tp[0].ToVector2(), tp[2].ToVector2(), LinearColor::Red);
//		DrawLine(tp[1].ToVector2(), tp[2].ToVector2(), LinearColor::Red);
//	}
//}

void WindowsRSI::DrawPrimitive(std::vector<Vector4>* InVertex, std::vector<int> *InIndex)
{
	static float repA = (float)ScreenSize.Y / (float)ScreenSize.X;
	//static float d = 1.f / tanf(Math::Deg2Rad(_fov) * 0.5f);
	static float d = 1.f / tanf(Math::Deg2Rad(60) * 0.5f);
	Matrix4x4 finMat = _uniformMat[0] * _uniformMat[1] * _uniformMat[2];
	int triangleCount = InIndex->size() / 3;
	for (int t = 0; t < triangleCount; t++)
	{
		Vector4 tp[3];
		tp[0] = InVertex->at(InIndex->at(t * 3));
		tp[1] = InVertex->at(InIndex->at(t * 3 + 1));
		tp[2] = InVertex->at(InIndex->at(t * 3 + 2)); 

		for (int ti = 0; ti < 3; ti++)
		{
			tp[ti] = finMat * tp[ti];
			float repW = 1.f / tp[ti].W;
			tp[ti].X *= repW;
			tp[ti].Y *= repW;
			tp[ti].Z *= repW;
		}

		Vector3 edge1 = (tp[1] - tp[0]).ToVector3();
		Vector3 edge2 = (tp[2] - tp[0]).ToVector3();
		Vector3 faceNormal = edge2.Cross(edge1).Normalize();
		static Vector3 cameraDir = -Vector3::UnitZ;
		if (cameraDir.Dot(faceNormal) < 0.f)
			continue;

		for (int ti = 0; ti < 3; ti++)
		{
			float repZ = 1.f / -tp[ti].Z;
			tp[ti].Y = tp[ti].Y * d * repZ;
			tp[ti].X = tp[ti].X * d * repZ * repA;

			tp[ti].X *= (ScreenSize.X * 0.5f);
			tp[ti].Y *= (ScreenSize.Y * 0.5f);
		}

		DrawLine(tp[0].ToVector2(), tp[1].ToVector2(), LinearColor::Red);
		DrawLine(tp[0].ToVector2(), tp[2].ToVector2(), LinearColor::Red);
		DrawLine(tp[1].ToVector2(), tp[2].ToVector2(), LinearColor::Red);
	}
}

void WindowsRSI::SetPixel(const ScreenPoint& InPos, const LinearColor& InColor)
{
	if (BlendingMode == BlendingModes::Opaque)
	{
		SetPixelOpaque(InPos, InColor);
	}
	else if (BlendingMode == BlendingModes::AlphaBlending)
	{
		SetPixelAlphaBlending(InPos, InColor);
	}
}
