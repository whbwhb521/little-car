#pragma once
#ifndef SOMEMODEL_H
#define SOMEMODEL_H

#include "Vertex.h"
#include <vector>
#include <map>
#include <string>
using namespace DirectX;

template<class VertexType = VertexPosNormalTex>
struct MeshData
{
	std::vector<VertexType> vertexVec;
	std::vector<WORD> indexVec;
};

template<class VertexType = VertexPosNormalTex>
MeshData<VertexType> CreateBox(float width = 2.0f, float height = 2.0f, float depth = 2.0f, UINT startindex = 0,
	const XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

template<class VertexType = VertexPosNormalTex>
MeshData<VertexType> CreatePlane(float centerX = 0.0f, float centerY = 0.0f, float centerZ = 0.0f,
	float width = 10.0f, float depth = 10.0f, float texU = 1.0f, float texV = 1.0f,
	const XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

template<class VertexType = VertexPosNormalTex>
MeshData<VertexType> CreateSphere(float radius = 1.0f, UINT levels = 20, UINT slices = 20,
	const XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

template<class VertexType = VertexPosNormalTex>
MeshData<VertexType> CreateWheel(float radius = 1.0f, float width = 0.5f, UINT slices = 20, UINT startindex = 0, float Degree = XM_2PI,
	const XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

template<class VertexType = VertexPosNormalTex>
MeshData<VertexType> CreateArm(float size = 1.0f);

template<class VertexType = VertexPosNormalTex>
MeshData<VertexType> CreateHandle(float size = 0.2);

template<class VertexType = VertexPosNormalTex>
MeshData<VertexType> CreateChair(float UnderPlaneSize = 1.0f, float ChairSize = 0.5f);

template<class VertexType = VertexPosNormalTex>
MeshData<VertexType> CreateComputerBack();
//-----------------------------------------------------------------------------------

struct VertexData
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT4 tangent;
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT2 tex;
};

// 根据目标顶点类型选择性将数据插入
template<class VertexType>
void InsertVertexData(VertexType& vertexDst, const VertexData& vertexSrc)
{
	static std::string semanticName;
	static const std::map<std::string, std::pair<size_t, size_t>> semanticSizeMap = {
		{"POSITION", std::pair<size_t, size_t>(0, 12)},
		{"NORMAL", std::pair<size_t, size_t>(12, 24)},
		{"TANGENT", std::pair<size_t, size_t>(24, 40)},
		{"COLOR", std::pair<size_t, size_t>(40, 56)},
		{"TEXCOORD", std::pair<size_t, size_t>(56, 64)}
	};

	for (size_t i = 0; i < ARRAYSIZE(VertexType::inputLayout); i++)
	{
		semanticName = VertexType::inputLayout[i].SemanticName;
		const auto& range = semanticSizeMap.at(semanticName);
		memcpy_s(reinterpret_cast<char*>(&vertexDst) + VertexType::inputLayout[i].AlignedByteOffset,
			range.second - range.first,
			reinterpret_cast<const char*>(&vertexSrc) + range.first,
			range.second - range.first);
	}
}

template<class VertexType>
inline MeshData<VertexType> CreateBox(float width, float height, float depth, UINT startindex,
	const XMFLOAT4& color)
{
	MeshData<VertexType> meshdata;
	meshdata.vertexVec.resize(24);
	VertexData vd[24];
	float w2 = width / 2, h2 = height / 2, d2 = depth / 2;
	vd[0].pos = XMFLOAT3(w2, -h2, -d2);
	vd[1].pos = XMFLOAT3(w2, h2, -d2);
	vd[2].pos = XMFLOAT3(w2, h2, d2);
	vd[3].pos = XMFLOAT3(w2, -h2, d2);
	vd[4].pos = XMFLOAT3(-w2, -h2, d2);
	vd[5].pos = XMFLOAT3(-w2, h2, d2);
	vd[6].pos = XMFLOAT3(-w2, h2, -d2);
	vd[7].pos = XMFLOAT3(-w2, -h2, -d2);
	vd[8].pos = XMFLOAT3(-w2, h2, -d2);
	vd[9].pos = XMFLOAT3(-w2, h2, d2);
	vd[10].pos = XMFLOAT3(w2, h2, d2);
	vd[11].pos = XMFLOAT3(w2, h2, -d2);
	vd[12].pos = XMFLOAT3(w2, -h2, -d2);
	vd[13].pos = XMFLOAT3(w2, -h2, d2);
	vd[14].pos = XMFLOAT3(-w2, -h2, d2);
	vd[15].pos = XMFLOAT3(-w2, -h2, -d2);
	vd[16].pos = XMFLOAT3(w2, -h2, d2);
	vd[17].pos = XMFLOAT3(w2, h2, d2);
	vd[18].pos = XMFLOAT3(-w2, h2, d2);
	vd[19].pos = XMFLOAT3(-w2, -h2, d2);
	vd[20].pos = XMFLOAT3(-w2, -h2, -d2);
	vd[21].pos = XMFLOAT3(-w2, h2, -d2);
	vd[22].pos = XMFLOAT3(w2, h2, -d2);
	vd[23].pos = XMFLOAT3(w2, -h2, -d2);

	for (UINT i = 0; i < 24; i++)
	{
		vd[i].color = color;
	}
	for (UINT i = 0; i < 4; i++)
	{
		vd[i].normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
		vd[i + 4].normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);
		vd[i + 8].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vd[i + 12].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
		vd[i + 16].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
		vd[i + 20].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	}
	for (UINT i = 0; i < 6; i++)
	{
		vd[i * 4].tex = XMFLOAT2(0.0f, 1.0f);
		vd[i * 4 + 1].tex = XMFLOAT2(0.0f, 0.0f);
		vd[i * 4 + 2].tex = XMFLOAT2(1.0f, 0.0f);
		vd[i * 4 + 3].tex = XMFLOAT2(1.0f, 1.0f);
	}
	for (UINT i = 0; i < 24; i++)
	{
		InsertVertexData(meshdata.vertexVec[i], vd[i]);
	}

	meshdata.indexVec =
	{
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4,
		8, 9, 10, 10, 11, 8,
		12, 13, 14, 14, 15, 12,
		16, 17, 18, 18, 19, 16,
		20, 21, 22, 22, 23, 20
	};
	for (UINT i = 0; i < meshdata.indexVec.size(); i++)
	{
		meshdata.indexVec[i] = meshdata.indexVec[i] + startindex;
	}

	return meshdata;
}

template<class VertexType>
inline MeshData<VertexType> CreatePlane(float centerX, float centerY, float centerZ,
	float width, float depth, float texU, float texV,
	const XMFLOAT4& color)
{
	MeshData<VertexType> meshdata;
	meshdata.vertexVec.resize(4);
	VertexData vd[4];
	DWORD vIndex = 0;
	vd[vIndex].pos = XMFLOAT3(centerX - width / 2, centerY, centerZ - depth / 2);
	vd[vIndex].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vd[vIndex].tex = XMFLOAT2(0.0f, texV);
	vIndex++;
	vd[vIndex].pos = XMFLOAT3(centerX - width / 2, centerY, centerZ + depth / 2);
	vd[vIndex].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vd[vIndex].tex = XMFLOAT2(0.0f, 0.0f);
	vIndex++;
	vd[vIndex].pos = XMFLOAT3(centerX + width / 2, centerY, centerZ + depth / 2);
	vd[vIndex].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vd[vIndex].tex = XMFLOAT2(texU, 0.0f);
	vIndex++;
	vd[vIndex].pos = XMFLOAT3(centerX + width / 2, centerY, centerZ - depth / 2);
	vd[vIndex].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vd[vIndex].tex = XMFLOAT2(texU, texV);
	for (int i = 0; i < 4; i++) 
	{
		InsertVertexData(meshdata.vertexVec[i], vd[i]);
	}

	meshdata.indexVec = { 0, 1, 2, 2, 3, 0 };

	return meshdata;
}

template<class VertexType>
inline MeshData<VertexType> CreateSphere(float radius, UINT levels, UINT slices,
	const XMFLOAT4& color)
{
	MeshData<VertexType> meshdata;
	UINT vertexCount = 2 + (levels - 1) * (slices + 1);
	UINT indexCount = 6 * (levels - 1) * slices;
	meshdata.vertexVec.resize(vertexCount);
	meshdata.indexVec.resize(indexCount);
	VertexData *vd = new VertexData[vertexCount];

	float phi = 0.0f, theta = 0.0f;
	float per_phi = XM_PI / levels;
	float per_theta = XM_2PI / slices;
	float x, y, z;
	DWORD vIndex = 0, iIndex = 0;
	vd[vIndex].pos = XMFLOAT3(0.0f, radius, 0.0f);
	vd[vIndex].normal =  XMFLOAT3(0.0f, 1.0f, 0.0f);
	vd[vIndex].tex =  XMFLOAT2(0.0f, 0.0f);
	vd[vIndex].color = color;
	vIndex++;
	for (UINT i = 1; i < levels; ++i)
	{
		phi = per_phi * i;
		for (UINT j = 0; j <= slices; j++) {
			theta = per_theta * j;
			x = radius * sinf(phi) * cosf(theta);
			y = radius * cosf(phi);
			z = radius * sinf(phi) * sinf(theta);
			// 计算出局部坐标、法向量、Tangent向量和纹理坐标
			XMFLOAT3 pos = XMFLOAT3(x, y, z), normal;
			//球体的法线与原点到pos的向量同方向
			XMStoreFloat3(&normal, XMVector3Normalize(XMLoadFloat3(&pos)));

			vd[vIndex].pos = pos;
			vd[vIndex].normal = normal;
			vd[vIndex].tex =  XMFLOAT2(theta / XM_2PI, phi / XM_PI);
			vd[vIndex].color = color;
			vIndex++;
		}
	}

	vd[vIndex].pos = XMFLOAT3(0.0f, -radius, 0.0f);
	vd[vIndex].normal =  XMFLOAT3(0.0f, -1.0, 0.0f);
	vd[vIndex].tex =  XMFLOAT2(0.0f, 1.0f);
	vd[vIndex].color = color;

	for (UINT i = 0; i < vertexCount; i++)
	{
		InsertVertexData(meshdata.vertexVec[i], vd[i]);
	}

	//放入索引
	if (levels > 1)
	{
		for (UINT j = 1; j <= slices; ++j)
		{
			meshdata.indexVec[iIndex++] = 0;
			meshdata.indexVec[iIndex++] = j % (slices + 1) + 1;
			meshdata.indexVec[iIndex++] = j;
		}
	}


	for (UINT i = 1; i < levels - 1; ++i)
	{
		for (UINT j = 1; j <= slices; ++j)
		{
			meshdata.indexVec[iIndex++] = (i - 1) * (slices + 1) + j;
			meshdata.indexVec[iIndex++] = (i - 1) * (slices + 1) + j % (slices + 1) + 1;
			meshdata.indexVec[iIndex++] = i * (slices + 1) + j % (slices + 1) + 1;

			meshdata.indexVec[iIndex++] = i * (slices + 1) + j % (slices + 1) + 1;
			meshdata.indexVec[iIndex++] = i * (slices + 1) + j;
			meshdata.indexVec[iIndex++] = (i - 1) * (slices + 1) + j;
		}
	}

	// 逐渐放入索引
	if (levels > 1)
	{
		for (UINT j = 1; j <= slices; ++j)
		{
			meshdata.indexVec[iIndex++] = (levels - 2) * (slices + 1) + j;
			meshdata.indexVec[iIndex++] = (levels - 2) * (slices + 1) + j % (slices + 1) + 1;
			meshdata.indexVec[iIndex++] = (levels - 1) * (slices + 1) + 1;
		}
	}
	delete vd;
	return meshdata;
}

template<class VertexType>
inline MeshData<VertexType> CreateWheel(float radius, float width, UINT slice, UINT startindex, float Degree,
	const XMFLOAT4& color)
{
	MeshData<VertexType> meshdata;
	UINT vertexCount = 4 * (slice+1) + 2;
	UINT indexCount = slice * 12;
	meshdata.vertexVec.resize(vertexCount);
	meshdata.indexVec.resize(indexCount);
	VertexData* vd = new VertexData[vertexCount];
	float theta;
	float per = Degree / slice;
	float y, z;
	DWORD vIndex = 0, iIndex = 0;
	//左轮中心
	vd[vIndex].pos = XMFLOAT3(-width / 2, 0.0f, 0.0f);
	vd[vIndex].normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);
	vd[vIndex].tex =  XMFLOAT2(0.5f, 0.5f);
	vd[vIndex].color = color;
	vIndex++;
	//右轮中心
	vd[vIndex].pos = XMFLOAT3(width / 2, 0.0f, 0.0f);
	vd[vIndex].normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
	vd[vIndex].tex = XMFLOAT2(0.5f, 0.5f);
	vd[vIndex].color = color;
	vIndex++;
	XMFLOAT3 pos, normal;
	//顶点
	for (UINT i = 0; i < 4; i++)
	{
		for (UINT j = 0; j <= slice; j++)
		{
			theta = per * j;
			y = radius * cosf(theta);
			z = radius * sinf(theta);
			if (i == 0) normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);
			else if (i == 1) normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
			else
			{
				pos = XMFLOAT3(0.0f, y, z), normal;
				XMStoreFloat3(&normal, XMVector3Normalize(XMLoadFloat3(&pos)));
			}
			vd[vIndex].pos = XMFLOAT3(i % 2 == 0 ? -width / 2 : width / 2, y, z);
			vd[vIndex].normal = normal;
			vd[vIndex].tex = XMFLOAT2(0.5f - 0.5f * cosf(theta),0.5f + 0.5f * sinf(theta));
			vd[vIndex].color = color;
			vIndex++;
		}
	}
	for (UINT i = 0; i < vertexCount; i++)
	{
		InsertVertexData(meshdata.vertexVec[i], vd[i]);
	}

	//索引
	for (UINT i = 2; i < slice + 2; i++)
	{
		meshdata.indexVec[iIndex++] = 0;
		meshdata.indexVec[iIndex++] = i + 1;
		meshdata.indexVec[iIndex++] = i;
	}
	for (UINT i = slice + 3; i < 2 * slice + 3; i++)
	{
		meshdata.indexVec[iIndex++] = 1;
		meshdata.indexVec[iIndex++] = i;
		meshdata.indexVec[iIndex++] = i + 1;
	}
	for (UINT i = 2 * slice + 4; i < 3 * slice + 4; i++)
	{
		meshdata.indexVec[iIndex++] = i;
		meshdata.indexVec[iIndex++] = i + slice + 2;
		meshdata.indexVec[iIndex++] = i + slice + 1;

		meshdata.indexVec[iIndex++] = i;
		meshdata.indexVec[iIndex++] = i + 1;
		meshdata.indexVec[iIndex++] = i + 2 + slice;
	}
	for (UINT i = 0; i < indexCount; i++)
	{
		meshdata.indexVec[i] = meshdata.indexVec[i] + startindex;
	}

	delete vd;
	return meshdata;
}


template<class VertexType>
inline MeshData<VertexType> CreateArm(float size)
{
	MeshData<VertexType> meshdata;
	MeshData<VertexType> meshdataBox1;
	MeshData<VertexType> meshdataJD1;
	MeshData<VertexType> meshdataBox2;
	MeshData<VertexType> meshdataJD2;
	//不转
	meshdataBox1 = CreateBox<VertexType>(size, 5.0f * size, size, 0, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));

	//第一个节点旋转90度--顺时针 XMFLOAT3(-0.5f, 2.5f, 0.0f)
	meshdataJD1 = CreateWheel<VertexType>(size, size, 10, meshdataBox1.vertexVec.size(), XM_PIDIV2);
	XMMATRIX RY = XMMatrixRotationY(-XM_PIDIV2);
	XMMATRIX RZ = XMMatrixRotationZ(-XM_PIDIV2);
	XMMATRIX PY = XMMatrixTranslation(-size / 2.0f, 2.5f * size, 0.0f);
	for (int i = 0; i < meshdataJD1.vertexVec.size(); i++)
	{
		XMStoreFloat3(&meshdataJD1.vertexVec[i].pos, XMVector3Transform(XMLoadFloat3(&meshdataJD1.vertexVec[i].pos), RY));
		XMStoreFloat3(&meshdataJD1.vertexVec[i].normal, XMVector3Transform(XMLoadFloat3(&meshdataJD1.vertexVec[i].normal), RY));
		XMStoreFloat3(&meshdataJD1.vertexVec[i].pos, XMVector3Transform(XMLoadFloat3(&meshdataJD1.vertexVec[i].pos), RZ));
		XMStoreFloat3(&meshdataJD1.vertexVec[i].normal, XMVector3Transform(XMLoadFloat3(&meshdataJD1.vertexVec[i].normal), RZ));
		XMStoreFloat3(&meshdataJD1.vertexVec[i].pos, XMVector3Transform(XMLoadFloat3(&meshdataJD1.vertexVec[i].pos), PY));
	}
	//第一个箱子转90度--逆时针 
	meshdataBox2 = CreateBox<VertexType>(size, 5.0f * size, size, meshdataBox1.vertexVec.size() + meshdataJD1.vertexVec.size(),
		XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));

	PY = XMMatrixTranslation(-3.0f * size, 3.0f * size, 0.0f);
	for (int i = 0; i < meshdataBox2.vertexVec.size(); i++)
	{
		XMStoreFloat3(&meshdataBox2.vertexVec[i].pos, XMVector3Transform(XMLoadFloat3(&meshdataBox2.vertexVec[i].pos), RZ));
		XMStoreFloat3(&meshdataBox2.vertexVec[i].normal, XMVector3Transform(XMLoadFloat3(&meshdataBox2.vertexVec[i].normal), RZ));
		XMStoreFloat3(&meshdataBox2.vertexVec[i].pos, XMVector3Transform(XMLoadFloat3(&meshdataBox2.vertexVec[i].pos), PY));
	}
	//勺子
	RY = XMMatrixRotationY(-XM_PIDIV2);
	PY = XMMatrixTranslation(-5.5f * size, 2.5f * size, 0.0f);
	meshdataJD2 = CreateWheel<VertexType>(size, size*2, 10, 2 * meshdataBox1.vertexVec.size() + meshdataJD1.vertexVec.size(), XM_PI);
	for (int i = 0; i < meshdataJD2.vertexVec.size(); i++)
	{
		XMStoreFloat3(&meshdataJD2.vertexVec[i].pos, XMVector3Transform(XMLoadFloat3(&meshdataJD2.vertexVec[i].pos), RY));
		XMStoreFloat3(&meshdataJD2.vertexVec[i].normal, XMVector3Transform(XMLoadFloat3(&meshdataJD2.vertexVec[i].normal), RY));
		XMStoreFloat3(&meshdataJD2.vertexVec[i].pos, XMVector3Transform(XMLoadFloat3(&meshdataJD2.vertexVec[i].pos), PY));
	}

	meshdata.vertexVec.insert(meshdata.vertexVec.end(), meshdataBox1.vertexVec.begin(), meshdataBox1.vertexVec.end());
	meshdata.vertexVec.insert(meshdata.vertexVec.end(), meshdataJD1.vertexVec.begin(), meshdataJD1.vertexVec.end());
	meshdata.vertexVec.insert(meshdata.vertexVec.end(), meshdataBox2.vertexVec.begin(), meshdataBox2.vertexVec.end());
	meshdata.vertexVec.insert(meshdata.vertexVec.end(), meshdataJD2.vertexVec.begin(), meshdataJD2.vertexVec.end());

	meshdata.indexVec.insert(meshdata.indexVec.end(), meshdataBox1.indexVec.begin(), meshdataBox1.indexVec.end());
	meshdata.indexVec.insert(meshdata.indexVec.end(), meshdataJD1.indexVec.begin(), meshdataJD1.indexVec.end());
	meshdata.indexVec.insert(meshdata.indexVec.end(), meshdataBox2.indexVec.begin(), meshdataBox2.indexVec.end());
	meshdata.indexVec.insert(meshdata.indexVec.end(), meshdataJD2.indexVec.begin(), meshdataJD2.indexVec.end());

	RZ = XMMatrixRotationZ(XM_PIDIV4);
	RY = XMMatrixRotationY(XM_PIDIV2);
	for (int i = 0; i < meshdata.vertexVec.size(); i++)
	{
		XMStoreFloat3(&meshdata.vertexVec[i].pos, XMVector3Transform(XMLoadFloat3(&meshdata.vertexVec[i].pos), RZ));
		XMStoreFloat3(&meshdata.vertexVec[i].normal, XMVector3Transform(XMLoadFloat3(&meshdata.vertexVec[i].normal), RZ));
		XMStoreFloat3(&meshdata.vertexVec[i].pos, XMVector3Transform(XMLoadFloat3(&meshdata.vertexVec[i].pos), RY));
		XMStoreFloat3(&meshdata.vertexVec[i].normal, XMVector3Transform(XMLoadFloat3(&meshdata.vertexVec[i].normal), RY));
	}

	return meshdata;
}

template<class VertexType>
inline MeshData<VertexType> CreateHandle(float size)
{
	MeshData<VertexType> meshdata;
	MeshData<VertexType> meshdata_Sphere = CreateSphere<VertexType>(size, 10, 10, XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f));
	MeshData<VertexType> meshdata_Wheel = CreateWheel<VertexType>(size / 3.0f, 10.0f*size, 10, meshdata_Sphere.vertexVec.size(), XM_2PI, XMFLOAT4(1.0f, 0.7f, 0.3f, 1.0f));

	XMMATRIX PY = XMMatrixTranslation(0.0f, 5.0f*size, 0.0f);
	for (int i = 0; i < meshdata_Sphere.vertexVec.size(); i++)
	{
		XMStoreFloat3(&meshdata_Sphere.vertexVec[i].pos, XMVector3Transform(XMLoadFloat3(&meshdata_Sphere.vertexVec[i].pos), PY));
	}
	meshdata.vertexVec.insert(meshdata.vertexVec.end(), meshdata_Sphere.vertexVec.begin(), meshdata_Sphere.vertexVec.end());
	meshdata.indexVec.insert(meshdata.indexVec.end(), meshdata_Sphere.indexVec.begin(), meshdata_Sphere.indexVec.end());
	XMMATRIX RZ = XMMatrixRotationZ(-XM_PIDIV2);
	for (int i = 0; i < meshdata_Wheel.vertexVec.size(); i++)
	{
		XMStoreFloat3(&meshdata_Wheel.vertexVec[i].pos, XMVector3Transform(XMLoadFloat3(&meshdata_Wheel.vertexVec[i].pos), RZ));
		XMStoreFloat3(&meshdata_Wheel.vertexVec[i].normal, XMVector3Transform(XMLoadFloat3(&meshdata_Wheel.vertexVec[i].normal), RZ));
	}
	meshdata.vertexVec.insert(meshdata.vertexVec.end(), meshdata_Wheel.vertexVec.begin(), meshdata_Wheel.vertexVec.end());
	meshdata.indexVec.insert(meshdata.indexVec.end(), meshdata_Wheel.indexVec.begin(), meshdata_Wheel.indexVec.end());
	return meshdata;
}

template<class VertexType>
inline MeshData<VertexType> CreateChair(float UnderPlaneSize, float ChairSize)
{
	MeshData<VertexType> meshdata;
	MeshData<VertexType> meshdata_UnderPlane = CreateBox<VertexType>(UnderPlaneSize, 0.05f, 2 * UnderPlaneSize, 0,  XMFLOAT4(0.4f, 0.2f, 0.2f, 1.0f));
	MeshData<VertexType> meshdata_Chair;
	MeshData<VertexType> meshdata_temp = CreateBox<VertexType>(ChairSize, 0.1f, ChairSize, meshdata_UnderPlane.vertexVec.size(), XMFLOAT4(1.0f, 0.5f, 0.2f, 1.0f));
	XMMATRIX PY = XMMatrixTranslation(0.0f, 0.05f, -UnderPlaneSize / 2.0f);
	for (int i = 0; i < meshdata_temp.vertexVec.size(); i++)
	{
		XMStoreFloat3(&meshdata_temp.vertexVec[i].pos, XMVector3Transform(XMLoadFloat3(&meshdata_temp.vertexVec[i].pos), PY));
	}
	meshdata_Chair.vertexVec.insert(meshdata_Chair.vertexVec.end(), meshdata_temp.vertexVec.begin(), meshdata_temp.vertexVec.end());
	meshdata_Chair.indexVec.insert(meshdata_Chair.indexVec.end(), meshdata_temp.indexVec.begin(), meshdata_temp.indexVec.end());
	meshdata_temp = CreateBox<VertexType>(ChairSize, ChairSize, 0.1f, meshdata_UnderPlane.vertexVec.size() + meshdata_temp.vertexVec.size(), XMFLOAT4(1.0f, 0.5f, 0.2f, 1.0f));
	PY = XMMatrixTranslation(0.0f, 0.2f, -UnderPlaneSize / 2.0f - ChairSize / 2.0f+0.05f);
	for (int i = 0; i < meshdata_temp.vertexVec.size(); i++)
	{
		XMStoreFloat3(&meshdata_temp.vertexVec[i].pos, XMVector3Transform(XMLoadFloat3(&meshdata_temp.vertexVec[i].pos), PY));
	}
	meshdata_Chair.vertexVec.insert(meshdata_Chair.vertexVec.end(), meshdata_temp.vertexVec.begin(), meshdata_temp.vertexVec.end());
	meshdata_Chair.indexVec.insert(meshdata_Chair.indexVec.end(), meshdata_temp.indexVec.begin(), meshdata_temp.indexVec.end());

	meshdata.vertexVec.insert(meshdata.vertexVec.end(), meshdata_UnderPlane.vertexVec.begin(), meshdata_UnderPlane.vertexVec.end());
	meshdata.indexVec.insert(meshdata.indexVec.end(), meshdata_UnderPlane.indexVec.begin(), meshdata_UnderPlane.indexVec.end());
	meshdata.vertexVec.insert(meshdata.vertexVec.end(), meshdata_Chair.vertexVec.begin(), meshdata_Chair.vertexVec.end());
	meshdata.indexVec.insert(meshdata.indexVec.end(), meshdata_Chair.indexVec.begin(), meshdata_Chair.indexVec.end());
	return meshdata;
}

template<class VertexType>
MeshData<VertexType> CreateComputerBack() 
{
	MeshData<VertexType> meshdata;
	MeshData<VertexType> meshdata_temp = CreateBox<VertexType>(0.5f,0.5f,0.05f,0, XMFLOAT4(0.5f, 0.2f, 0.0f, 1.0f));
	meshdata.vertexVec.insert(meshdata.vertexVec.end(), meshdata_temp.vertexVec.begin(), meshdata_temp.vertexVec.end());
	meshdata.indexVec.insert(meshdata.indexVec.end(), meshdata_temp.indexVec.begin(), meshdata_temp.indexVec.end());
	meshdata_temp = CreateBox<VertexType>(0.5f, 0.1f, 0.2f, meshdata_temp.vertexVec.size(), XMFLOAT4(0.5f, 0.2f, 0.0f, 1.0f));
	XMMATRIX PY = XMMatrixTranslation(0.0f, -0.3f, -0.075f);
	for (int i = 0; i < meshdata_temp.vertexVec.size(); i++)
	{
		XMStoreFloat3(&meshdata_temp.vertexVec[i].pos, XMVector3Transform(XMLoadFloat3(&meshdata_temp.vertexVec[i].pos), PY));
	}
	meshdata.vertexVec.insert(meshdata.vertexVec.end(), meshdata_temp.vertexVec.begin(), meshdata_temp.vertexVec.end());
	meshdata.indexVec.insert(meshdata.indexVec.end(), meshdata_temp.indexVec.begin(), meshdata_temp.indexVec.end());
	return meshdata;
}
#endif // !SOMEMODEL_H