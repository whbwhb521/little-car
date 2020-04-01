#include "Base.hlsli"

// ������ɫ��(3D)
float4 PS_3D(VertexPosHWNormalTex pIn) : SV_Target
{
    float4 texColor = g_Tex.Sample(g_SamLinear, pIn.Tex);
    // ��׼��������
    pIn.NormalW = normalize(pIn.NormalW);

    // ����ָ���۾�������
    float3 toEyeW = normalize(g_EyePosW - pIn.PosW);

    // ��ʼ��Ϊ0 
    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 A = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 D = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 S = float4(0.0f, 0.0f, 0.0f, 0.0f);
    int i;
    // ǿ��չ��ѭ���Լ���ָ����
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

    if (iscube)
    {
        texColor = g_TexCube.Sample(g_SamLinear, pIn.PosL);
    }
    float4 litColor = texColor * (ambient + diffuse) + spec;
    litColor.a = texColor.a * g_Material.Diffuse.a;

    return litColor;
}