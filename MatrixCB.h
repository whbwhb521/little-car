#pragma once
#include "LightHelper.h"
struct WorldMatrixCB
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX worldInvTranspose;
};

struct ViewMatrixCB
{
	DirectX::XMMATRIX view;
	DirectX::XMFLOAT4 eyePos;
};

struct ProjMatrixCB
{
	DirectX::XMMATRIX proj;
};

struct LightMatrixCB
{
	DirectX::XMMATRIX Shadow;
	DirectionalLight dirLight[5];
	PointLight pointLight[5];
	SpotLight spotLight[5];
	Material material;
	int numDirLight;
	int numPointLight;
	int numSpotLight;
	float pad;		// 打包保证16字节对齐
};

struct SkyWorldViewProjMatrixCB 
{
	DirectX::XMMATRIX worldViewProj;
};

struct IsCubeCB 
{
	int isCube;
	DirectX::XMFLOAT3 g_pad;
};

struct IsShadowCB
{
	Material material;
	int isShadow;
	DirectX::XMFLOAT3 g_pad;
};