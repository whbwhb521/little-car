#pragma once
#ifndef RENDERSTATES_H
#define RENDERSTATES_H

#include <wrl/client.h>
#include <d3d11_1.h>

class RenderStates
{
public:
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	static bool IsInit();
	static void InitAll(ID3D11Device* device);
	// 使用ComPtr无需手工释放

public:
	static ComPtr<ID3D11RasterizerState> RSWireframe;		            //线框模式
	static ComPtr<ID3D11RasterizerState> RSNoCull;			            //无背面裁剪模式
	static ComPtr<ID3D11RasterizerState> RSCullClockWise;	            //顺时针裁剪模式                    //初始状态

	static ComPtr<ID3D11SamplerState> SSLinearWrap;			            // 采样器状态：线性过滤
	static ComPtr<ID3D11SamplerState> SSAnistropicWrap;		            // 采样器状态：各项异性过滤

	static ComPtr<ID3D11BlendState> BSTransparent;

	static ComPtr<ID3D11DepthStencilState> DSSLessEqual;		        // 深度/模板状态：允许绘制深度值相等的像素
	static ComPtr<ID3D11DepthStencilState> DSSNoDoubleBlend;
};


#endif