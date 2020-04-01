#pragma once
#ifndef RENDERTOTEXTURE_H
#define RENDERTOTEXTURE_H
#include <d3d11_1.h>
#include <DirectXMath.h>
#include <string>
#include <wrl/client.h>

class RenderToTexture 
{
public:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	RenderToTexture();
	~RenderToTexture();
	//初始化
	void InitResource(ID3D11Device* device, int texWidth, int texHeight);
	//开始渲染
	void StartRender(ComPtr<ID3D11DeviceContext> deviceContext);
	//结束渲染
	void EndRender(ComPtr<ID3D11DeviceContext> deviceContext);
	// 获取渲染好的纹理
	ID3D11ShaderResourceView* GetOutputTexture();
private:
	D3D11_VIEWPORT                      m_viewport;             // 输出视口
	ComPtr<ID3D11ShaderResourceView>	m_pOutputTextureSRV;	// 输出的SRV
	ComPtr<ID3D11RenderTargetView>		m_pOutputTextureRTV;	// 输出的RTV
	ComPtr<ID3D11DepthStencilView>		m_pOutputTextureDSV;	// 输出的深度/模板视图

	ComPtr<ID3D11RenderTargetView>		m_pCacheRTV;		    // 保存原来的RTV
	ComPtr<ID3D11DepthStencilView>		m_pCacheDSV;		    // 保存原来的DSV
	D3D11_VIEWPORT						m_CacheViewPort;	    // 保存原来的视口
};

#endif