#include "Base.hlsli"

// ������ɫ��(3D)
VertexPosHWNormalTex VS_3D(VertexPosNormalTex vIn)
{
    VertexPosHWNormalTex vOut;

    matrix viewProj = mul(g_View, g_Proj);
    float4 posW = mul(float4(vIn.PosL, 1.0f), g_World);
    float3 normalW = mul(vIn.NormalL, (float3x3) g_WorldInvTranspose);
    float3 posL = vIn.PosL;
    // ����ǰ�ڻ�����Ӱ
    [flatten]
    if (g_IsShadow)
    {
        posW = mul(posW, g_Shadow);
    }

    vOut.PosH = mul(posW, viewProj);
    vOut.PosW = posW.xyz;
    vOut.NormalW = normalW;
    vOut.PosL = posL;
    vOut.Tex = vIn.Tex;
    return vOut;
}