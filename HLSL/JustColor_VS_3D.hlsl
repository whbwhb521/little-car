#include "Base.hlsli"
// ������ɫ��(3D)
VertexPosHWColorNormal VS_3D(VertexPosColorNormal vIn)
{
    VertexPosHWColorNormal vOut;
    matrix viewProj = mul(g_View, g_Proj);
    float4 posW = mul(float4(vIn.PosL, 1.0f), g_World);
    float4 posH = mul(posW, viewProj);
    // ����ǰ�ڻ�����Ӱ
    [flatten]
    if (g_IsShadow)
    {
        posW = mul(posW, g_Shadow);
    }
    vOut.PosH = posH;
    vOut.PosW = posW.xyz;
    vOut.NormalW = mul(vIn.NormalL, (float3x3) g_WorldInvTranspose);
    vOut.Color = vIn.Color;
    return vOut;
}