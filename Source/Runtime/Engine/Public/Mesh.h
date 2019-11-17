#pragma once

class Mesh
{
public:
	Mesh();
	Mesh(Vector4* inVertex, const int& inVertexCnt, int* index, const int& indexCnt);
		
	std::vector<Vector4> getVertex() const { return _vertex; }
	int getVertexCnt() const { return _vertexCnt; }

	std::vector<int> getIndex() const { return _index; }
	int getIndexCnt() const { return _indexCnt; }
	
	void setMesh(Vector4* inVertex, const int& inVertexCnt, int* index, const int& indexCnt);

private:
	
	std::vector<Vector4> _vertex;
	int _vertexCnt;

	std::vector<int> _index;
	int _indexCnt;
};
