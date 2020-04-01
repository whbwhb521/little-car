#pragma once
#ifndef VERTEX_H
#define VERTEX_H

#include <d3d11_1.h>
#include <DirectXMath.h>

struct VertexPos 
{
	DirectX::XMFLOAT3 pos;
	static const D3D11_INPUT_ELEMENT_DESC inputLayout[1];
};

struct VertexPosColor
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT4 color;
	static const D3D11_INPUT_ELEMENT_DESC inputLayout[2];
};

struct VertexPosTex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 tex;
	static const D3D11_INPUT_ELEMENT_DESC inputLayout[2];
};

struct VertexPosNormalTex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 tex;
	static const D3D11_INPUT_ELEMENT_DESC inputLayout[3];
};

struct  PointSprite
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT2 Size;
	static const D3D11_INPUT_ELEMENT_DESC inputLayout[2];
};

struct VertexPosColorNormal 
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT4 color;
	static const D3D11_INPUT_ELEMENT_DESC inputLayout[3];
};
#endif