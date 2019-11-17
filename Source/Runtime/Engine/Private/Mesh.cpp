#include "Precompiled.h"
#include "Mesh.h"

Mesh::Mesh()
{
	_vertexCnt = 0;
	_indexCnt = 0;
}

Mesh::Mesh(Vector4* inVertex, const int& inVertexCnt, int* index, const int& indexCnt)
{
	_vertexCnt = inVertexCnt;
	_indexCnt = indexCnt;

	for (int i = 0; i < inVertexCnt; i++)
		_vertex.push_back(inVertex[i]);

	for (int i = 0; i < indexCnt; i++)
		_index.push_back(index[i]);
}

void Mesh::setMesh(Vector4* inVertex, const int& inVertexCnt, int* index, const int& indexCnt)
{
	_vertexCnt = inVertexCnt;
	_indexCnt = indexCnt;

	for (int i = 0; i < inVertexCnt; i++)
		_vertex.push_back(inVertex[i]);

	for (int i = 0; i < indexCnt; i++)
		_index.push_back(index[i]);
}

