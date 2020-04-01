#include "LightHelper.hlsli"

Texture2D g_Tex : register(t0);
TextureCube g_TexCube : register(t1);
SamplerState g_SamLinear : register(s0);

cbuffer WorldMatrixCB : register(b0)
{
	matrix g_World;
	matrix g_WorldInvTranspose;
}

cbuffer ViewMatrixCB : register(b1)
{
	matrix g_View;
	float3 g_EyePosW;
}

cbuffer ProjMatrixCB : register(b2)
{
	matrix g_Proj;
}

cbuffer LightCB : register(b3)
{
	matrix g_Shadow;
	DirectionalLight g_DirLight[5];
	PointLight g_PointLight[5];
	SpotLight g_SpotLight[5];  
	int g_NumDirLight;
	int g_NumPointLight;
	int g_NumSpotLight;
}
cbuffer IsCube : register(b4)
{
	int iscube;
	float3 g_pad;
}

cbuffer IsShadow : register(b5)
{
	Material g_Material; //为了方便，暂时让大家用同种材质
	int g_IsShadow;
	float3 g_pad2;
}

struct VertexPosColorNormal
{
	float3 PosL : POSITION;
	float3 NormalL : NORMAL;
	float4 Color : COLOR;
};

struct VertexPosHWColorNormal
{
	float4 PosH : SV_POSITION;
	float3 PosW : POSITION;
	float4 Color : COLOR;
	float3 NormalW : NORMAL;
};

struct VertexPosNormalTex
{
	float3 PosL : POSITION;
	float3 NormalL : NORMAL;
	float2 Tex : TEXCOORD;
};

struct VertexPosTex
{
	float3 PosL : POSITION;
	float2 Tex : TEXCOORD;
};

struct VertexPosHWNormalTex
{
	float4 PosH : SV_POSITION;
	float3 PosL : LOCAL_POSITION; //加入局部坐标用于textcube
	float3 PosW : POSITION; 
	float3 NormalW : NORMAL; 
	float2 Tex : TEXCOORD;
};

struct VertexPosHTex
{
	float4 PosH : SV_POSITION;
	float2 Tex : TEXCOORD;
};

struct PointSprite
{
	float3 PosW : POSITION;
	float2 SizeW : SIZE;
};