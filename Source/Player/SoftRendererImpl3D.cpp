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
	static float b = 1.f / tanf(_fov * 0.5f);

	for (int t = 0; t < triangleCount; t++)
	{
		Vector4 p0 = v[i[t * 3]];
		Vector4 p1 = v[i[t * 3 + 1]];
		Vector4 p2 = v[i[t * 3 + 2]];

		p0 = FinalMatrix * p0;
		p1 = FinalMatrix * p1;
		p2 = FinalMatrix * p2;

		/*for (int ti = 0; ti < 3; ti++)
		{
			tp[ti] = sibal;
		}*/

		RSI->DrawLine(p0.ToVector2(), p1.ToVector2(), LinearColor::Red);
		RSI->DrawLine(p0.ToVector2(), p2.ToVector2(), LinearColor::Red);
		RSI->DrawLine(p1.ToVector2(), p2.ToVector2(), LinearColor::Red);
	}
}

void SoftRendererImpl3D::UpdateImpl(float DeltaSeconds)
{
	static Vector3 cubePos = Vector3::Zero;
	static float cubeRotationY = 0.f;
	static float cubeRotationX = 0.f;
	static float cubeRotationZ = 0.f;
	static float cubeScaleXYZ = 100.f;

	static float moveSpeed = 100.f;
	static float rotateSpeed = 180.f;
	cubePos += Vector3::UnitZ * InputManager.GetYAxis() * moveSpeed * DeltaSeconds;
	cubeRotationY += InputManager.GetXAxis() * rotateSpeed * DeltaSeconds;


	float cy, sy, cp, sp, cr, sr;
	Math::GetSinCos(cy, sy, cubeRotationY);
	Math::GetSinCos(cp, sp, cubeRotationX);
	Math::GetSinCos(cr, sr, cubeRotationZ);

	Matrix4x4 tMat(Vector4::UnitX, Vector4::UnitY, Vector4::UnitZ, Vector4(cubePos));
	Matrix4x4 rMat(
		Vector4(cr * cy + sr * sp * sy, cy * sr - cr * sp * sy, -cp * sy, false),
		Vector4(-sr * cp, cr * cp, -sp, false),
		Vector4(cr * sy - sr * sp * cy, sr * sy + cr * sp * cy, cp * cy, false),
		Vector4::UnitW);

	Matrix4x4 sMat(Vector4::UnitX * cubeScaleXYZ, Vector4::UnitY * cubeScaleXYZ, Vector4::UnitZ * cubeScaleXYZ, Vector4::UnitW);
	Matrix4x4 mMat = tMat * rMat * sMat;

	static Camera camera;
	camera.SetPosition(Vector3(0, 500.f, -500.f));
	static Vector3 cameraPos(0.f, 500.f, -500.f);

	Vector3 viewZ = (cubePos - cameraPos).Normalize();
	Vector3 viewX = Vector3::UnitY.Cross(viewZ).Normalize();
	if (viewX.IsZero())
	{
		viewX = Vector3::UnitX;
	}
	Vector3 viewY = viewX.Cross(viewZ).Normalize();

	Matrix4x4 virMat = Matrix4x4(Vector4(viewX, false), Vector4(viewY, false), Vector4(viewZ, false), Vector4::UnitW).Tranpose();
	Matrix4x4 vitMat = Matrix4x4(Vector4::UnitX, Vector4::UnitY, Vector4::UnitZ, Vector4(-cameraPos));
	Matrix4x4 vMat = virMat * vitMat;

	// 최종 행렬
	FinalMatrix = vMat * mMat;
}
