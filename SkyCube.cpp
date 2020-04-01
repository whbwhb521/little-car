#include "SkyCube.h"
#include "d3dUtil.h"
#include "DXTrace.h"
#include "DDSTextureLoader.h"
#include "SomeModel.h"

using namespace DirectX;

SkyCube::SkyCube() :
	m_pTextureCubeSRV(), cb_Sky(), m_IndexCount()
{
}

void SkyCube::SetSkyCubeInfo(ID3D11Device* device,
	ID3D11DeviceContext* deviceContext,
	const std::wstring& cubemapFilename,
	float skySphereRadius,		// 天空球半径
	bool generateMips)
{
	//以dds结尾
	if (cubemapFilename.substr(cubemapFilename.size()-3) == L"dds")
	{
		HR(CreateDDSTextureFromFile(device,
			generateMips ? deviceContext : nullptr,
			cubemapFilename.c_str(),
			nullptr,
			m_pTextureCubeSRV.GetAddressOf()));
	}
	else 
	{
		HR(CreateWICTexture2DCubeFromFile(
			device,
			deviceContext,
			cubemapFilename,
			nullptr,
			m_pTextureCubeSRV.GetAddressOf(),
			generateMips
		));
	}
	InitResource(device, skySphereRadius);
}

void SkyCube::SetSkyCubeInfo(ID3D11Device* device,
	ID3D11DeviceContext* deviceContext,
	const std::vector<std::wstring>& cubemapFilenames,
	float skySphereRadius,		// 天空球半径
	bool generateMips)
{
	HR(CreateWICTexture2DCubeFromFile(
		device,
		deviceContext,
		cubemapFilenames,
		nullptr,
		m_pTextureCubeSRV.GetAddressOf(),
		generateMips
	));
	InitResource(device, skySphereRadius);
}

void SkyCube::InitResource(ID3D11Device* device, float skySphereRadius)
{
	auto sphere = CreateSphere<VertexPos>(skySphereRadius);

	// 顶点缓冲区创建
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(VertexPos) * (UINT)sphere.vertexVec.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = sphere.vertexVec.data();

	HR(device->CreateBuffer(&vbd, &InitData, &m_pVertexBuffer));

	// 索引缓冲区创建
	m_IndexCount = (UINT)sphere.indexVec.size();

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(WORD) * m_IndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.StructureByteStride = 0;
	ibd.MiscFlags = 0;

	InitData.pSysMem = sphere.indexVec.data();

	HR(device->CreateBuffer(&ibd, &InitData, &m_pIndexBuffer));
}

void SkyCube::Draw(ID3D11DeviceContext* deviceContext, const Camera& camera)
{
	UINT strides = sizeof(XMFLOAT3);
	UINT offsets = 0;
	//设置缓冲区
	deviceContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &strides, &offsets);
	deviceContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	//获取相机位置
	XMFLOAT3 pos = camera.GetPosition();
	cb_Sky.worldViewProj = XMMatrixTranspose(XMMatrixTranslation(pos.x, pos.y, pos.z) * camera.GetViewProjXM());

	ComPtr<ID3D11Buffer> cBuffer = nullptr;
	deviceContext->VSGetConstantBuffers(0, 1, cBuffer.GetAddressOf());
	//设置SRV
	deviceContext->PSSetShaderResources(0, 1, m_pTextureCubeSRV.GetAddressOf());
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(deviceContext->Map(cBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	memcpy_s(mappedData.pData, sizeof(SkyWorldViewProjMatrixCB), &cb_Sky, sizeof(SkyWorldViewProjMatrixCB));
	deviceContext->Unmap(cBuffer.Get(), 0);
	
	deviceContext->DrawIndexed(m_IndexCount, 0, 0);
}
