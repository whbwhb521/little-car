#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <d3d11_1.h>
#include <DirectXMath.h>
#include <d2d1.h>
#include <wrl/client.h>
#include "SomeModel.h"
#include "MatrixCB.h"
#include "DXTrace.h"

class GameObject
{
public:
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	GameObject();
	DirectX::XMFLOAT3 GetPosition() const;
	void SetWorldMatrix(const DirectX::XMFLOAT4X4& world);
	void XM_CALLCONV SetWorldMatrix(DirectX::FXMMATRIX world);
	DirectX::XMFLOAT4X4 GetWorldMatrix() const;
	template<class VertexType>
	void SetObjInfo(ID3D11Device* device,const MeshData<VertexType>& meshData);
	void SetObjInfo(ID3D11Device* device,const PointSprite &vertexes , UINT drawCount);
	void DrawIndexed(ID3D11DeviceContext* deviceContext);
	void Draw(ID3D11DeviceContext* deviceContext);
	UINT GetIndexCount();
	void SetObjTexture(ID3D11ShaderResourceView* texture, bool isCube);
private:
	WorldMatrixCB cb_world;
	DirectX::XMFLOAT4X4 m_WorldMatrix;
	IsCubeCB cb_iscube;
	ComPtr<ID3D11ShaderResourceView> m_pTexture_cube;	// 纹理
	ComPtr<ID3D11ShaderResourceView> m_pTexture;		// 纹理

	ComPtr<ID3D11Buffer> m_pVertexBuffer;				// 顶点缓冲区
	ComPtr<ID3D11Buffer> m_pIndexBuffer;				// 索引缓冲区
	UINT m_VertexStride;								// 顶点字节大小
	UINT m_drawCount;								     //画的点数
	UINT isCube;                                        //是否texturecube
};

template<class VertexType>
inline void GameObject::SetObjInfo(ID3D11Device* device, const MeshData<VertexType>& meshData)
{
	m_pVertexBuffer.Reset();
	m_pIndexBuffer.Reset();
	//顶点缓冲区描述
	m_VertexStride = sizeof(VertexType);
	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = (UINT)meshData.vertexVec.size() * m_VertexStride;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = meshData.vertexVec.data();
	device->CreateBuffer(&vbd, &InitData, m_pVertexBuffer.GetAddressOf());

	//索引缓冲区描述
	m_drawCount = (UINT)meshData.indexVec.size();
	D3D11_BUFFER_DESC ibd;
	ZeroMemory(&ibd, sizeof(ibd));
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = m_drawCount * sizeof(DWORD);
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	InitData.pSysMem = meshData.indexVec.data();
	device->CreateBuffer(&ibd, &InitData, m_pIndexBuffer.GetAddressOf());
}

#endif