#include "Base.hlsli"

// 像素着色器(3D)
float4 PS_3D(VertexPosHWColorNormal pIn) : SV_Target
{

    // 标准化法向量
    pIn.NormalW = normalize(pIn.NormalW);

    // 顶点指向眼睛的向量
    float3 toEyeW = normalize(g_EyePosW - pIn.PosW);
    // 初始化为0 
    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 A = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 D = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 S = float4(0.0f, 0.0f, 0.0f, 0.0f);
    int i;
    // 强制展开循环以减少指令数
    DirectionalLight dirLight;
    [unroll]
    for (i = 0; i < g_NumDirLight; ++i)
    {
        dirLight = g_DirLight[i];
        if (!g_IsShadow)
        {
            ComputeDirectionalLight(g_Material, dirLight, pIn.NormalW, toEyeW, A, D, S);
        }
        ambient += A;
        diffuse += D;
        spec += S;
    }

    PointLight pointLight;
    [unroll]
    for (i = 0; i < g_NumPointLight; ++i)
    {
        pointLight = g_PointLight[i];
        ComputePointLight(g_Material, pointLight, pIn.PosW, pIn.NormalW, toEyeW, A, D, S);
        ambient += A;
        diffuse += D;
        spec += S;
    }


    SpotLight spotLight;
    [unroll]
    for (i = 0; i < g_NumSpotLight; ++i)
    {
        spotLight = g_SpotLight[i];
        if (!g_IsShadow)
        {
            ComputeSpotLight(g_Material, spotLight, pIn.PosW, pIn.NormalW, toEyeW, A, D, S);
        }
        ambient += A;
        diffuse += D;
        spec += S;
    }

    float4 litColor = pIn.Color * (ambient + diffuse) + spec;
    litColor.a = pIn.Color.a * g_Material.Diffuse.a;

    return litColor;
}