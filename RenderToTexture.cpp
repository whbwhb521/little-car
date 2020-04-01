#include "RenderToTexture.h"

RenderToTexture::RenderToTexture() {}

RenderToTexture::~RenderToTexture() {}

void RenderToTexture::InitResource(ID3D11Device* device, int texWidth, int texHeight)
{
	ComPtr<ID3D11Texture2D> texture;
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = texWidth;
	texDesc.Height = texHeight;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	device->CreateTexture2D(&texDesc, nullptr, texture.ReleaseAndGetAddressOf());
	// 创建纹理对应的渲染目标视图

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	rtvDesc.Format = texDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;
	device->CreateRenderTargetView(texture.Get(), &rtvDesc, m_pOutputTextureRTV.GetAddressOf());

	// 创建纹理对应的着色器资源视图
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.TextureCube.MipLevels = -1;	// 使用所有的mip等级

	device->CreateShaderResourceView(texture.Get(), &srvDesc, m_pOutputTextureSRV.GetAddressOf());

	// 创建与纹理等宽高的深度/模板缓冲区和对应的视图
	texDesc.Width = texWidth;
	texDesc.Height = texHeight;
	texDesc.MipLevels = 0;
	texDesc.ArraySize = 1;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ComPtr<ID3D11Texture2D> depthTex;
	device->CreateTexture2D(&texDesc, nullptr, depthTex.GetAddressOf());

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Format = texDesc.Format;
	dsvDesc.Flags = 0;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	device->CreateDepthStencilView(depthTex.Get(), &dsvDesc, m_pOutputTextureDSV.GetAddressOf());

	//初始化视口
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;
	m_viewport.Width = static_cast<float>(texWidth);
	m_viewport.Height = static_cast<float>(texHeight);
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;

}

void RenderToTexture::StartRender(ComPtr<ID3D11DeviceContext> deviceContext)
{
	// 缓存渲染目标和深度模板视图
	deviceContext->OMGetRenderTargets(1, m_pCacheRTV.GetAddressOf(), m_pCacheDSV.GetAddressOf());
	// 缓存视口
	UINT num_Viewports = 1;
	deviceContext->RSGetViewports(&num_Viewports, &m_CacheViewPort);


	// 清空缓冲区
	float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	deviceContext->ClearRenderTargetView(m_pOutputTextureRTV.Get(), black);
	deviceContext->ClearDepthStencilView(m_pOutputTextureDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	// 设置渲染目标和深度模板视图
	deviceContext->OMSetRenderTargets(1, m_pOutputTextureRTV.GetAddressOf(), m_pOutputTextureDSV.Get());
	// 设置视口
	deviceContext->RSSetViewports(1, &m_viewport);
}

void RenderToTexture::EndRender(ComPtr<ID3D11DeviceContext> deviceContext)
{
	// 恢复默认设定
	deviceContext->RSSetViewports(1, &m_CacheViewPort);
	deviceContext->OMSetRenderTargets(1, m_pCacheRTV.GetAddressOf(), m_pCacheDSV.Get());


	// 清空临时缓存的渲染目标视图和深度模板视图
	m_pCacheDSV.Reset();
	m_pCacheRTV.Reset();
}

ID3D11ShaderResourceView* RenderToTexture::GetOutputTexture() 
{
	return m_pOutputTextureSRV.Get();
}