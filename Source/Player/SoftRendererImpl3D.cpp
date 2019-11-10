
#include "Precompiled.h"
#include "SoftRendererImpl3D.h"
#include "SoftRenderer.h"

SoftRendererImpl3D::SoftRendererImpl3D(SoftRenderer* InOwner)
{
	RSI = InOwner->RSI.get();
	ScreenSize = InOwner->CurrentScreenSize;
	InputManager = InOwner->GetInputManager();
}

SoftRendererImpl3D::~SoftRendererImpl3D()
{
}


void SoftRendererImpl3D::RenderFrameImpl()
{
	assert(RSI != nullptr && RSI->IsInitialized() && !ScreenSize.HasZero());

	const int vertexCount = 24;
	static Vector4 v[vertexCount] = {
		// Front 
		Vector4(0.5f, -0.5f, 0.5f),
		Vector4(0.5f, 0.5f, 0.5f),
		Vector4(0.5f, 0.5f, -0.5f),
		Vector4(0.5f, -0.5f, -0.5f),
		// Left
		Vector4(-0.5f, -0.5f, 0.5f),
		Vector4(-0.5f, 0.5f, 0.5f),
		Vector4(0.5f, 0.5f, 0.5f),
		Vector4(0.5f, -0.5f, 0.5f),
		// Right
		Vector4(0.5f, -0.5f, -0.5f),
		Vector4(0.5f, 0.5f, -0.5f),
		Vector4(-0.5f, 0.5f, -0.5f),
		Vector4(-0.5f, -0.5f, -0.5f),
		// Back
		Vector4(-0.5f, -0.5f, -0.5f),
		Vector4(-0.5f, 0.5f, -0.5f),
		Vector4(-0.5f, 0.5f, 0.5f),
		Vector4(-0.5f, -0.5f, 0.5f),
		// Top
		Vector4(0.5f, 0.5f, 0.5f),
		Vector4(-0.5f, 0.5f, 0.5f),
		Vector4(-0.5f, 0.5f, -0.5f),
		Vector4(0.5f, 0.5f, -0.5f),
		// Bottom
		Vector4(-0.5f, -0.5f, 0.5f),
		Vector4(0.5f, -0.5f, 0.5f),
		Vector4(0.5f, -0.5f, -0.5f),
		Vector4(-0.5f, -0.5f, -0.5f)
	};

	const int triangleCount = 12;
	const int indexCount = triangleCount * 3;
	static int i[indexCount] = {
	 0, 2, 1, 0, 3, 2,
	 4, 6, 5, 4, 7, 6,
	 8, 10, 9, 8, 11, 10,
	 12, 14, 13, 12, 15, 14,
	 16, 18, 17, 16, 19, 18,
	 20, 22, 21, 20, 23, 22
	};

	static float a = (float)ScreenSize.X / (float)ScreenSize.Y;
	static float repA = (float)ScreenSize.Y / (float)ScreenSize.X;
	static float d = 1.f / tanf(Math::Deg2Rad(_fov) * 0.5f);

	for (int t = 0; t < triangleCount; t++)
	{
		Vector4 tp[3];
		tp[0] = v[i[t * 3]];
		tp[1] = v[i[t * 3 + 1]];
		tp[2] = v[i[t * 3 + 2]];

		for (int ti = 0; ti < 3; ti++)
		{
			tp[ti] = FinalMatrix * tp[ti];
			float repW = 1.f / tp[ti].W;
			tp[ti].X *= repW;
			tp[ti].Y *= repW;
			tp[ti].Z *= repW;
		}

		// Backface Culling
		Vector3 edge1 = (tp[1] - tp[0]).ToVector3();
		Vector3 edge2 = (tp[2] - tp[0]).ToVector3();
		Vector3 faceNormal = -edge1.Cross(edge2).Normalize();
		static Vector3 cameraDir = -Vector3::UnitZ;
		if (cameraDir.Dot(faceNormal) > 0.f)
			continue;

		for (int ti = 0; ti < 3; ti++)
		{
			float repZ = 1.f / -tp[ti].Z;
			tp[ti].Y = tp[ti].Y * d * repZ;
			tp[ti].X = tp[ti].X * d * repZ * repA;

			tp[ti].X *= (ScreenSize.X * 0.5f);
			tp[ti].Y *= (ScreenSize.Y * 0.5f);
		}

		RSI->DrawLine(tp[0].ToVector2(), tp[1].ToVector2(), LinearColor::Red);
		RSI->DrawLine(tp[0].ToVector2(), tp[2].ToVector2(), LinearColor::Red);
		RSI->DrawLine(tp[1].ToVector2(), tp[2].ToVector2(), LinearColor::Red);
	}
}

void SoftRendererImpl3D::UpdateImpl(float DeltaSeconds)
{
	static float moveSpeed = 100.f;
	static float rotateSpeed = 180.f;

	static GameObject cube = GameObject();
	cube.AddPosition(Vector3(0, 0, InputManager.GetYAxis() * moveSpeed * DeltaSeconds));
	cube.AddRotate(Vector3(0.f, InputManager.GetXAxis() * moveSpeed * DeltaSeconds, 0.f));
	cube.SetScale(100.f);
	static Vector3 cameraPos = Vector3(0, -500, -500);
	Vector3 viewZ = (cameraPos - cube.getPosition()).Normalize();
	Vector3 viewX = Vector3::UnitY.Cross(viewZ).Normalize();
	if (viewX.IsZero())
	{
		viewX = Vector3::UnitX;
	}
	Vector3 viewY = viewX.Cross(viewZ).Normalize();

	Matrix4x4 virMat = Matrix4x4(Vector4(viewX, false), Vector4(viewY, false), Vector4(viewZ, false), Vector4::UnitW).Tranpose();
	Matrix4x4 vitMat = Matrix4x4(Vector4::UnitX, Vector4::UnitY, Vector4::UnitZ, Vector4(-cameraPos));
	Matrix4x4 vMat = virMat * vitMat;

	static float repA = (float)ScreenSize.Y / (float)ScreenSize.X;
	static float d = 1.f / tanf(Math::Deg2Rad(_fov) * 0.5f);
	static float n = 5.5f;
	static float f = 100.f;
	float repNF = 1.f / (n - f);
	float k = f * repNF;
	float l = f * n * repNF;
	Matrix4x4 pMat(
		Vector4::UnitX * repA * d,
		Vector4::UnitY * d,
		Vector4(0.f, 0.f, k, -1.f),
		Vector4(0.f, 0.f, l, 0.f));

	FinalMatrix = pMat * vMat * cube.GetTRS();
}
