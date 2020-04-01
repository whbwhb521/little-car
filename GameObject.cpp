#include "GameObject.h"
#include "d3dUtil.h"
#include "DXTrace.h"

using namespace DirectX;
GameObject::GameObject()
	: m_drawCount(), 
	m_VertexStride(), 
	cb_iscube(), 
	cb_world(),
	isCube()
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
}

XMFLOAT3 GameObject::GetPosition() const 
{
	return XMFLOAT3(m_WorldMatrix(3, 0), m_WorldMatrix(3, 1), m_WorldMatrix(3, 2));
}

XMFLOAT4X4 GameObject::GetWorldMatrix() const
{
	return m_WorldMatrix;
}

void GameObject::SetWorldMatrix(const XMFLOAT4X4& world) 
{
	m_WorldMatrix = world;
}

void XM_CALLCONV GameObject::SetWorldMatrix(FXMMATRIX world) 
{
	XMStoreFloat4x4(&m_WorldMatrix, world);
}

UINT GameObject::GetIndexCount() 
{
	return m_drawCount;
}

void GameObject::SetObjTexture(ID3D11ShaderResourceView* texture,bool iscube)
{
	isCube = iscube;
	//设置纹理
	if (iscube)
	{
		m_pTexture_cube = texture;
	}
	else 
	{
		m_pTexture = texture;
	}
	
	
}

void GameObject::SetObjInfo(ID3D11Device* device, const PointSprite& vertexes, UINT drawCount)
{
	m_pVertexBuffer.Reset();
	m_pIndexBuffer.Reset();
	m_VertexStride = sizeof(PointSprite);
	m_drawCount = drawCount;
	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_IMMUTABLE;	// 数据不可修改
	vbd.ByteWidth = sizeof(vertexes);
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	// 新建顶点缓冲区
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = &vertexes;
	HR(device->CreateBuffer(&vbd, &InitData, m_pVertexBuffer.GetAddressOf()));
}


void GameObject::DrawIndexed(ID3D11DeviceContext* deviceContext)
{
	UINT strides = m_VertexStride;
	UINT offsets = 0;
	//设置缓冲区
	deviceContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &strides, &offsets);
	deviceContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	ComPtr<ID3D11Buffer> cBuffer = nullptr;
	deviceContext->VSGetConstantBuffers(0, 1, cBuffer.GetAddressOf());

	XMMATRIX W = XMLoadFloat4x4(&m_WorldMatrix);
	cb_world.world = XMMatrixTranspose(W);
	cb_world.worldInvTranspose = XMMatrixInverse(nullptr, W);
	//更新常量缓冲区
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(deviceContext->Map(cBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	memcpy_s(mappedData.pData, sizeof(WorldMatrixCB), &cb_world, sizeof(WorldMatrixCB));
	deviceContext->Unmap(cBuffer.Get(), 0);

	deviceContext->PSGetConstantBuffers(4, 1, cBuffer.GetAddressOf());
	cb_iscube.isCube = isCube;
	HR(deviceContext->Map(cBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0,
		&mappedData));
	memcpy_s(mappedData.pData, sizeof(IsCubeCB), &cb_iscube, sizeof(IsCubeCB));
	deviceContext->Unmap(cBuffer.Get(), 0);

	//设置纹理
	deviceContext->PSSetShaderResources(0,1,m_pTexture.GetAddressOf());
	deviceContext->PSSetShaderResources(1, 1, m_pTexture_cube.GetAddressOf());
	deviceContext->DrawIndexed(m_drawCount, 0, 0);
}

void GameObject::Draw(ID3D11DeviceContext* deviceContext)
{
	UINT strides = m_VertexStride;
	UINT offsets = 0;
	//设置缓冲区
	deviceContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &strides, &offsets);
	ComPtr<ID3D11Buffer> cBuffer = nullptr;
	deviceContext->PSGetConstantBuffers(4, 1, cBuffer.GetAddressOf());
	cb_iscube.isCube = isCube;
	//更新常量缓冲区
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(deviceContext->Map(cBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0,
		&mappedData));
	memcpy_s(mappedData.pData, sizeof(IsCubeCB), &cb_iscube, sizeof(IsCubeCB));
	deviceContext->Unmap(cBuffer.Get(), 0);
	deviceContext->PSSetShaderResources(0, 1, m_pTexture.GetAddressOf());
	deviceContext->PSSetShaderResources(1, 1, m_pTexture_cube.GetAddressOf());
	deviceContext->Draw(m_drawCount,0);
}