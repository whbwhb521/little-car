#pragma once
#ifndef SKYCUBE_H
#define SKYCUBE_H
#include <wrl/client.h>
#include <vector>
#include <string>
#include "camera.h"
#include "MatrixCB.h"

class SkyCube
{
public:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	SkyCube();
	void SetSkyCubeInfo(ID3D11Device* device,
		ID3D11DeviceContext* deviceContext,
		const std::wstring& cubemapFilename,
		float skySphereRadius,		// 天空球半径
		bool generateMips = false);

	void SetSkyCubeInfo(ID3D11Device* device,
		ID3D11DeviceContext* deviceContext,
		const std::vector<std::wstring>& cubemapFilenames,
		float skySphereRadius,		// 天空球半径
		bool generateMips = false);


	//绘制
	void Draw(ID3D11DeviceContext* deviceContext, const Camera& camera);

private:
	void InitResource(ID3D11Device* device, float skySphereRadius);

private:
	SkyWorldViewProjMatrixCB cb_Sky;
	ComPtr<ID3D11ShaderResourceView> m_pTextureCubeSRV;
	ComPtr<ID3D11Buffer> m_pVertexBuffer;
	ComPtr<ID3D11Buffer> m_pIndexBuffer;
	UINT m_IndexCount;
};

#endif