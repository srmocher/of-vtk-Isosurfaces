#include "marchingcubes.h"

MarchingCubes::MarchingCubes(float ***data,int *dims) {
	computeNormals = false;
	this->data = data;
	this->dims = dims;
	cout << "Creating normals array"<<endl;
	normals = new ofVec3f**[dims[0]];
	for (int i = 0;i < dims[0];i++)
	{
		cout<<"test"<<endl;
		normals[i] = new ofVec3f*[dims[1]];
		for (int j = 0;j < dims[1];j++)
		{
			normals[i][j] = new ofVec3f[dims[2]];
			for (int k = 0;k < dims[2];k++)
			{
				normals[i][j][k] = ofVec3f(0, 0, 0);
			}
		}
	}
	
		

}

void MarchingCubes::extractIsoSurface(Cube cube, float isoValue,int i,int j,int k)
{
	
	int cubeIndex = 0;

	ofVec3f verticesList[12],normalsList[12];
	int i1 = min(i + 1, dims[0]-1), j1 = min(j + 1, dims[1]-1), k1 = min(k + 1, dims[2]-1);
	cube.values[0] < isoValue? cubeIndex |= 1:cubeIndex |= 0;
	cube.values[1] < isoValue? cubeIndex |= 2:cubeIndex |= 0;
	cube.values[2] < isoValue? cubeIndex |= 4 : cubeIndex |= 0;
	cube.values[3] < isoValue? cubeIndex |= 8 : cubeIndex |= 0;
	cube.values[4] < isoValue? cubeIndex |= 16 : cubeIndex |= 0;
	cube.values[5] < isoValue? cubeIndex |= 32 : cubeIndex |= 0;
	cube.values[6] < isoValue? cubeIndex |= 64 : cubeIndex |= 0;
	cube.values[7] < isoValue? cubeIndex |= 128 : cubeIndex |= 0;

	if (edgeTable[cubeIndex] == 0)
		return; // all values above or below the current isoValue

	if (edgeTable[cubeIndex] & 1) {
		verticesList[0] = interpolateVertices(isoValue, cube.points[0], cube.points[1], cube.values[0], cube.values[1]);
		if(computeNormals)
			normalsList[0] = interpolateNormals(isoValue, cube.points[0], cube.points[1], cube.values[0], cube.values[1], i, j, k, i1, j, k);
	}
	if (edgeTable[cubeIndex] & 2) {
		verticesList[1] = interpolateVertices(isoValue, cube.points[1], cube.points[2], cube.values[1], cube.values[2]);
		if(computeNormals)
			normalsList[1] = interpolateNormals(isoValue, cube.points[1], cube.points[2], cube.values[1], cube.values[2], i1, j, k, i1, j1, k);
	}
	if (edgeTable[cubeIndex] & 4) {
		verticesList[2] = interpolateVertices(isoValue, cube.points[2], cube.points[3], cube.values[2], cube.values[3]);
		if(computeNormals)
			normalsList[2] = interpolateNormals(isoValue, cube.points[2], cube.points[3], cube.values[2], cube.values[3], i1, j1, k, i, j1, k);
	}
	if (edgeTable[cubeIndex] & 8) {
		verticesList[3] = interpolateVertices(isoValue, cube.points[3], cube.points[0], cube.values[3], cube.values[0]);
		if(computeNormals)
			normalsList[3] = interpolateNormals(isoValue, cube.points[3], cube.points[0], cube.values[3], cube.values[0],i, j1, k, i, j, k);
	}
	if (edgeTable[cubeIndex] & 16) {
		verticesList[4] =
			interpolateVertices(isoValue, cube.points[4], cube.points[5], cube.values[4], cube.values[5]);
		normalsList[4] = interpolateNormals(isoValue, cube.points[4], cube.points[5], cube.values[4], cube.values[5], i, j, k1, i1, j, k1);
	}
	if (edgeTable[cubeIndex] & 32) {
		verticesList[5] =
			interpolateVertices(isoValue, cube.points[5], cube.points[6], cube.values[5], cube.values[6]);
		if(computeNormals)
			normalsList[5] = interpolateNormals(isoValue, cube.points[5], cube.points[6], cube.values[5], cube.values[6], i1, j, k1, i1, j1, k1);
	}
	if (edgeTable[cubeIndex] & 64) {
		verticesList[6] =
			interpolateVertices(isoValue, cube.points[6], cube.points[7], cube.values[6], cube.values[7]);
		normalsList[6] = interpolateNormals(isoValue, cube.points[6], cube.points[7], cube.values[6], cube.values[7], i1, j1, k1, i, j1, k1);
	}
	if (edgeTable[cubeIndex] & 128) {
		verticesList[7] =
			interpolateVertices(isoValue, cube.points[7], cube.points[4], cube.values[7], cube.values[4]);
		if(computeNormals)
			normalsList[7] = interpolateNormals(isoValue, cube.points[7], cube.points[4], cube.values[7], cube.values[4], i, j1, k1, i, j, k1);
	}
	if (edgeTable[cubeIndex] & 256) {
		verticesList[8] = interpolateVertices(isoValue, cube.points[0], cube.points[4], cube.values[0], cube.values[4]);
		if(computeNormals)
			normalsList[8] = interpolateNormals(isoValue, cube.points[0], cube.points[4], cube.values[0], cube.values[4], i, j, k, i, j, k1);
	}
	if (edgeTable[cubeIndex] & 512) {
		verticesList[9] = interpolateVertices(isoValue, cube.points[1], cube.points[5], cube.values[1], cube.values[5]);
		if(computeNormals)
			normalsList[9] = interpolateNormals(isoValue, cube.points[1], cube.points[5], cube.values[1], cube.values[5], i1, j, k, i1, j, k1);
	}
	if (edgeTable[cubeIndex] & 1024) {
		verticesList[10] = interpolateVertices(isoValue, cube.points[2], cube.points[6], cube.values[2], cube.values[6]);
		if(computeNormals)
			normalsList[10] = interpolateNormals(isoValue, cube.points[2], cube.points[6], cube.values[2], cube.values[6], i1, j1, k, i1, j1, k1);
	}
	if (edgeTable[cubeIndex] & 2048) {
		verticesList[11] = interpolateVertices(isoValue, cube.points[3], cube.points[7], cube.values[3], cube.values[7]);
		if(computeNormals)
			normalsList[11] = interpolateNormals(isoValue, cube.points[3], cube.points[7], cube.values[3], cube.values[7], i, j1, k, i, j1, k1);
	}

	
	for (int i = 0;triTable[cubeIndex][i] != -1;i += 3) {
		Triangle t;
		t.vertices[0] = verticesList[triTable[cubeIndex][i]];
		t.vertices[1] = verticesList[triTable[cubeIndex][i + 1]];
		t.vertices[2] = verticesList[triTable[cubeIndex][i + 2]];
		triangles.push_back(t);		
		vertices.push_back(t.vertices[0]);
		vertices.push_back(t.vertices[1]);
		vertices.push_back(t.vertices[2]);
		if (computeNormals) {
			ofVec3f normal;
			normal = normalsList[triTable[cubeIndex][i]];
			Normals.push_back(normal);
			normal = normalsList[triTable[cubeIndex][i + 1]];
			Normals.push_back(normal);
			normal = normalsList[triTable[cubeIndex][i + 2]];
			Normals.push_back(normal);
		}
	}

	
	
}

//ofVec3f MarchingCubes::interpolateVertices(float isoValue, ofVec3f point1, ofVec3f point2, float point1Val, float point2Val)
//{
//	double mu;
//	ofVec3f p;
//
//	if (fabsf(isoValue - point1Val) < 0.00001)
//		return point1;
//	if (fabsf(isoValue - point2Val) < 0.00001)
//		return point2;
//	if (fabsf(point1Val - point2Val) < 0.00001)
//		return point1;
//	mu = (isoValue - point1Val) / (point2Val - point1Val);
//	p.x = point1.x + mu * (point2.x - point1.x);
//	p.y = point1.y + mu * (point2.y - point1.y);
//	p.z = point1.z + mu * (point2.z - point1.z);
//
//	return p;
//}

ofVec3f MarchingCubes::interpolateVertices(float isoValue, ofVec3f point1, ofVec3f point2, float point1Val, float point2Val)
{
	double u;
	ofVec3f p;

	u = (point1Val - isoValue) / (point1Val - point2Val);

	p = point1*(1 - u) + point2*u;

	return p;
}
vector<ofVec3f> MarchingCubes::getNormals()
{
	return Normals;
}

vector<Triangle> MarchingCubes::getTriangles()
{
	return triangles;
}

void MarchingCubes::setComputeNormals(bool val) {
	computeNormals = val;
}

void MarchingCubes::reset() {
	triangles.clear();
	Normals.clear();
	vertices.clear();
}

void MarchingCubes::computeNormal(int i, int j, int k)
{
	ofVec3f& n = normals[i][j] [k];
	n.set(data[min(dims[0]-1, i + 1)] [j] [k] - data[max(0, i - 1)] [j] [k],
		data[i] [min(dims[1]-1, j + 1)] [k] - data[i] [max(0, j - 1)] [k],
		data[i] [j] [min(dims[2]-1, k + 1)] - data[i] [j] [max(0, k - 1)]);

	n.normalize();
	normals[i][j][k] = n;
}

ofVec3f MarchingCubes::interpolateNormals(float isoValue, ofVec3f point1, ofVec3f point2, float point1Value, float point2Value,int i1,int j1, int k1,int i2,int j2, int k2)
{
	computeNormal(i1, j1, k1);
	computeNormal(i2, j2, k2);

	ofVec3f n1 = normals[i1][j1][k1];
	ofVec3f n2 = normals[i2][j2][k2];

	ofVec3f n;
	float t = (isoValue - point1Value) / (point2Value - point1Value);
	
	n = n1 + (n2 - n1) * t;
	return n;
}

MarchingCubes::~MarchingCubes()
{
	if(normals!=NULL)
	       delete normals;	
}
