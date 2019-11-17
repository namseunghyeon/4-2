
#include "Precompiled.h"
#include "SoftRendererImpl3D.h"
#include "SoftRenderer.h"

SoftRendererImpl3D::SoftRendererImpl3D(SoftRenderer* InOwner)
{
	RSI = InOwner->RSI.get();
	ScreenSize = InOwner->CurrentScreenSize;
	InputManager = InOwner->GetInputManager();

	// load resource -> load scene 
	AwakeScene();
}

SoftRendererImpl3D::~SoftRendererImpl3D()
{

}


void SoftRendererImpl3D::AwakeScene()
{
	_camera = Camera(_fov);
	_camera.SetPosition(Vector3(0.f, -500.f, -500.f));

	// cube Data

	// vetex set
	Vector4 v[] = {
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

	int vertexCnt = sizeof(v) / sizeof(Vector4);

	int index[] = {
	 0, 2, 1, 0, 3, 2,
	 4, 6, 5, 4, 7, 6,
	 8, 10, 9, 8, 11, 10,
	 12, 14, 13, 12, 15, 14,
	 16, 18, 17, 16, 19, 18,
	 20, 22, 21, 20, 23, 22
	};

	int indexCount = sizeof(index) / sizeof(int); /*tris cnt*/ //12 * 3;

	for (int i = 0; i < 5; i++)
	{
		GameObject cube;
		cube.SetScale(100.f);

		// set cube.
		cube.getMesh().setMesh(v, vertexCnt, index, indexCount);
		cube.SetScale(100);
		cube.AddPosition(Vector3(i * 100, i * 100, i*100));

		_objects.emplace_back(cube);
	}
}

void SoftRendererImpl3D::RenderFrameImpl()
{
	assert(RSI != nullptr && RSI->IsInitialized() && !ScreenSize.HasZero());

	// cam set
	_camera.setPMat(ScreenSize);

	// v, i
	//VertexData* v = nullptr;
	//int* index = nullptr;

	//// get cnt
	//*int vertexCnt = _cube.getMesh().getVertexCnt();
	//int indexCnt = _cube.getMesh().getIndexCnt();*/

	// vertex / index
	//v = new VertexData[vertexCnt];
	//index = new int[indexCnt];

	//// set value
	//for (int i = 0; i < vertexCnt; i++)
	//	v[i].Position = _cube.getMesh().getVertex()[i];

	//for (int i = 0; i < indexCnt; i++)
	//	index[i] = _cube.getMesh().getIndex()[i];

	//
	//Matrix4x4 m[3] = { _camera.getPMat(), _camera.getLookMat(_cube.getPosition()) ,_cube.GetTRS() };
	//RSI->SetUniformMatrix(m);
	//RSI->SetVertexBuffer(v);
	//RSI->SetIndexBuffer(index); 
	//RSI->DrawPrimitive(vertexCnt, indexCnt);

	//delete[] v;
	//delete[] index;

	for (int i = 0; i < _objects.size(); i++)
	{
		Matrix4x4 m[3] = { _camera.getPMat(), _camera.getLookMat(_objects[i].getPosition()) ,_objects[i].GetTRS() };
		RSI->SetUniformMatrix(m);
		RSI->DrawPrimitive(&_objects[i].getMesh().getVertex(), &_objects[i].getMesh().getIndex());
	}
}

void SoftRendererImpl3D::UpdateImpl(float DeltaSeconds)
{
	static float moveSpeed = 100.f;
	static float rotateSpeed = 180.f;

	_objects[0].AddPosition(Vector3(0, 0, InputManager.GetYAxis() * moveSpeed * DeltaSeconds));
	_objects[0].AddRotate(Vector3(0.f, InputManager.GetXAxis() * moveSpeed * DeltaSeconds, 0.f));
}
