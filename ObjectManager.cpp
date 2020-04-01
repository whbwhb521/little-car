#include "ObjectManager.h"
#include "DXTrace.h"
#include "d3dUtil.h"
#include "RenderStates.h"

ObjectManager::ObjectManager() : cb_light() {}

ObjectManager::~ObjectManager() {}

GameObject ObjectManager::GetBox() 
{
	return m_Box;
}

GameObject* ObjectManager::GetWheel() 
{
	return m_Wheel;
}

GameObject ObjectManager::GetPlane() 
{
	return m_plane;
}

GameObject ObjectManager::GetArm() 
{
	return m_Arm;
}

GameObject ObjectManager::GetBoard() 
{
	return m_Board;
}

GameObject ObjectManager::GetChair() 
{ 
	return m_Chair; 
}

GameObject ObjectManager::GetLeftHandle() 
{
	return m_LeftHandle;
}

GameObject ObjectManager::GetRightHandle() 
{
	return m_RightHandle;
}

GameObject ObjectManager::GetMyComputer() 
{
	return m_Computer;
}

GameObject ObjectManager::GetMyComputerBack() 
{
	return m_Computer_back;
}

void ObjectManager::InitShaderAndLayout(ComPtr<ID3D11Device> m_pd3dDevice)
{
	ComPtr<ID3DBlob> blob;
	//物体的布局和着色器
	// 创建顶点着色器(3D)
	HR(CreateShaderFromFile(L"HLSL\\Basic_VS_3D.cso", L"HLSL\\Basic_VS_3D.hlsl", "VS_3D", "vs_5_0", blob.ReleaseAndGetAddressOf()));
	HR(m_pd3dDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pVertexShader3D.GetAddressOf()));
	// 创建顶点布局(3D)
	HR(m_pd3dDevice->CreateInputLayout(VertexPosNormalTex::inputLayout, ARRAYSIZE(VertexPosNormalTex::inputLayout),
		blob->GetBufferPointer(), blob->GetBufferSize(), m_pVertexLayout3D.GetAddressOf()));
	// 创建像素着色器(3D)
	HR(CreateShaderFromFile(L"HLSL\\Basic_PS_3D.cso", L"HLSL\\Basic_PS_3D.hlsl", "PS_3D", "ps_5_0", blob.ReleaseAndGetAddressOf()));
	HR(m_pd3dDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pPixelShader3D.GetAddressOf()));

	//天空的布局和着色器
	HR(CreateShaderFromFile(L"HLSL\\Sky_VS.cso", L"HLSL\\Sky_VS.hlsl", "VS", "vs_5_0", blob.ReleaseAndGetAddressOf()));
	HR(m_pd3dDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_SkyVertexShader3D.GetAddressOf()));
	HR(m_pd3dDevice->CreateInputLayout(VertexPos::inputLayout, ARRAYSIZE(VertexPos::inputLayout),
		blob->GetBufferPointer(), blob->GetBufferSize(), m_SkyLayout3D.GetAddressOf()));
	HR(CreateShaderFromFile(L"HLSL\\Sky_PS.cso", L"HLSL\\Sky_PS.hlsl", "PS", "ps_5_0", blob.ReleaseAndGetAddressOf()));
	HR(m_pd3dDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_SkyPixelShader3D.GetAddressOf()));
	//木板布局和着色器
	HR(CreateShaderFromFile(L"HLSL\\Board_GS_3D.cso", L"HLSL\\Board_GS_3D.hlsl", "GS", "gs_5_0", blob.ReleaseAndGetAddressOf()));
	HR(m_pd3dDevice->CreateGeometryShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_BoardGeometryShader3D.GetAddressOf()));
	HR(CreateShaderFromFile(L"HLSL\\Board_VS_3D.cso", L"HLSL\\Board_VS_3D.hlsl", "VS", "vs_5_0", blob.ReleaseAndGetAddressOf()));
	HR(m_pd3dDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_BoardVertexShader3D.GetAddressOf()));
	HR(m_pd3dDevice->CreateInputLayout(PointSprite::inputLayout, ARRAYSIZE(PointSprite::inputLayout), blob->GetBufferPointer(),
		blob->GetBufferSize(), m_BoardLayout3D.GetAddressOf()));
	//座位和操纵杆
	HR(CreateShaderFromFile(L"HLSL\\JustColor_VS_3D.cso", L"HLSL\\JustColor_VS_3D.hlsl", "VS_3D", "vs_5_0", blob.ReleaseAndGetAddressOf()));
	HR(m_pd3dDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_ChairVertexShader3D.GetAddressOf()));
	HR(m_pd3dDevice->CreateInputLayout(VertexPosColorNormal::inputLayout, ARRAYSIZE(VertexPosColorNormal::inputLayout),
		blob->GetBufferPointer(), blob->GetBufferSize(), m_ChairLayout3D.GetAddressOf()));
	HR(CreateShaderFromFile(L"HLSL\\JustColor_PS_3D.cso", L"HLSL\\JustColor_PS_3D.hlsl", "PS_3D", "ps_5_0", blob.ReleaseAndGetAddressOf()));
	HR(m_pd3dDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_ChairPixelShader3D.GetAddressOf()));
}

void ObjectManager::InitObjResource(ComPtr<ID3D11Device> m_pd3dDevice, ComPtr<ID3D11DeviceContext> m_pd3dImmediateContext)
{   //创建缓冲区
	D3D11_BUFFER_DESC cbd;
	ZeroMemory(&cbd, sizeof(cbd));
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.ByteWidth = sizeof(WorldMatrixCB);
	HR(m_pd3dDevice->CreateBuffer(&cbd, nullptr, mbf_world.GetAddressOf()));
	cbd.ByteWidth = sizeof(LightMatrixCB);
	HR(m_pd3dDevice->CreateBuffer(&cbd, nullptr, mbf_light.GetAddressOf()));
	cbd.ByteWidth = sizeof(IsCubeCB);
	HR(m_pd3dDevice->CreateBuffer(&cbd, nullptr, mbf_cube.GetAddressOf()));

	//物体纹理  里面包含了世界坐标系缓冲区的更新一集视角坐标系缓冲区的更新
	//车子
	
	HR(CreateWICTexture2DCubeFromFile(m_pd3dDevice.Get(),m_pd3dImmediateContext.Get(), 
		std::vector<std::wstring> {
		L"Texture\\WJJ.png", L"Texture\\WJJ2.png",
			L"Texture\\wjj4.png", L"Texture\\wjj4.png",
			L"Texture\\wjj5.png", L"Texture\\WJJ3.png",}, nullptr,
		Objtexture.GetAddressOf()));
	auto meshdataBox = CreateBox(boxheight * 2, boxheight, boxheight * 4);
	auto meshdataWheel = CreateWheel(wheelraudis, wheelraudis / 4, 20);
	auto meshdataArm = CreateArm(0.2f);
	m_Box.SetObjInfo(m_pd3dDevice.Get(), meshdataBox);
	m_Box.SetObjTexture(Objtexture.Get(), true);
	m_Box.SetWorldMatrix(XMMatrixTranslation(0.0f, wheelraudis + boxheight / 2, 0.0f));

	HR(CreateWICTexture2DCubeFromFile(m_pd3dDevice.Get(), m_pd3dImmediateContext.Get(),
		L"Texture\\ARM.jpg", nullptr,
		Objtexture.GetAddressOf()));
	//手臂
	m_Arm.SetObjInfo(m_pd3dDevice.Get(), meshdataArm);
	m_Arm.SetObjTexture(Objtexture.Get(), true);
	m_Arm.SetWorldMatrix(XMMatrixTranslation(1.0f * cosf(3.0f * XM_PI / 8.0f), wheelraudis + boxheight + 0.4f, 1.0f * sinf(3.0f * XM_PI / 8.0f)));
	//轮子
	HR(CreateWICTextureFromFile(m_pd3dDevice.Get(), L"Texture\\wheel.jpg", nullptr,
		Objtexture.GetAddressOf()));
	for (int i = 0; i < 4; i++) 
	{
		m_Wheel[i].SetObjInfo(m_pd3dDevice.Get(), meshdataWheel);
		m_Wheel[i].SetObjTexture(Objtexture.Get(),false);
		m_Wheel[i].SetWorldMatrix(XMMatrixTranslation(
			i % 2 == 0 ? -boxheight - wheelraudis / 4 : boxheight + wheelraudis / 4,
			wheelraudis,
			i < 2 ? boxheight + wheelraudis / 4 : -boxheight - wheelraudis / 4));
	}

	//地面
	HR(CreateWICTextureFromFile(m_pd3dDevice.Get(), L"Texture\\CD.jpg", nullptr,
		Objtexture.GetAddressOf()));
	m_plane.SetObjInfo(m_pd3dDevice.Get(), CreatePlane(0.0f, 0.0f, 0.0f, 100.0f, 100.0f, 20.0f, 20.0f));
	m_plane.SetObjTexture(Objtexture.Get(),false);

	//木板
	HR(CreateWICTextureFromFile(m_pd3dDevice.Get(), L"Texture\\CZTS.png", nullptr,
		Objtexture.GetAddressOf()));
	m_Board.SetObjTexture(Objtexture.Get(), false);
	PointSprite vertexes[1];
	vertexes[0].Pos = XMFLOAT3(0.0f, 3.0f, 10.0f);
	vertexes[0].Size = XMFLOAT2(6.0f, 4.0f);
	m_Board.SetObjInfo(m_pd3dDevice.Get(), *vertexes,1);

	//座位
	m_Chair.SetObjInfo(m_pd3dDevice.Get(), CreateChair<VertexPosColorNormal>(0.4f, 0.3f));
	m_Chair.SetWorldMatrix(XMMatrixTranslation(0.0f, wheelraudis + boxheight + 0.0025f,0.0f));

	//握把
	m_LeftHandle.SetObjInfo(m_pd3dDevice.Get(), CreateHandle<VertexPosColorNormal>(0.04f));
	m_LeftHandle.SetWorldMatrix(XMMatrixTranslation(-0.15f, wheelraudis + boxheight, 0.15f));
	m_RightHandle.SetObjInfo(m_pd3dDevice.Get(), CreateHandle<VertexPosColorNormal>(0.04f));
	m_RightHandle.SetWorldMatrix(XMMatrixTranslation(0.15f, wheelraudis + boxheight, 0.15f));

	//电脑
	m_Computer.SetObjInfo(m_pd3dDevice.Get(), CreatePlane(0.0f, 0.0f, 0.0f, 0.4f, 0.4f, 1.0f, 1.0f));
	m_Computer_back.SetObjInfo(m_pd3dDevice.Get(), CreateComputerBack<VertexPosColorNormal>());

	// 初始化不会变化的值
	// 环境光
	/*cb_light.dirLight[0].ambient = XMFLOAT4(0.5f, 0.5f, 0.3f, 1.0f);
	cb_light.dirLight[0].diffuse = XMFLOAT4(0.3f, 0.3f, 0.2f, 1.0f);
	cb_light.dirLight[0].specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	cb_light.dirLight[0].direction = XMFLOAT3(10.0f, -10.0f, -10.0f);*/
	cb_light.dirLight[0].ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	cb_light.dirLight[0].diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	cb_light.dirLight[0].specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	cb_light.dirLight[0].direction = XMFLOAT3(0.0f, -1.0f, 0.0f);

	// 灯光
	cb_light.pointLight[0].position = XMFLOAT3(-50.0f, 30.0, 50.0f);
	cb_light.pointLight[0].ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	cb_light.pointLight[0].diffuse = XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
	cb_light.pointLight[0].specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	cb_light.pointLight[0].att = XMFLOAT3(0.0f, 0.1f, 0.0f);
	cb_light.pointLight[0].range = 150.0f;
	cb_light.numDirLight = 1;
	cb_light.numPointLight = 1;
	cb_light.numSpotLight = 0;
	// 初始化材质
	cb_light.material.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	cb_light.material.diffuse = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	cb_light.material.specular = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	cb_light.Shadow = XMMatrixTranspose(XMMatrixShadow(XMVectorSet(0.0f, 1.0f, 0.0f, -0.01f), XMVectorSet(-50.0f, 30.0, 50.0f, 1.0f)));
	//更新缓冲区
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(m_pd3dImmediateContext->Map(mbf_light.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0,
		&mappedData));
	memcpy_s(mappedData.pData, sizeof(LightMatrixCB), &cb_light, sizeof(LightMatrixCB));
	m_pd3dImmediateContext->Unmap(mbf_light.Get(), 0);
	m_pd3dImmediateContext->VSSetConstantBuffers(0, 1, mbf_world.GetAddressOf());
	m_pd3dImmediateContext->VSSetConstantBuffers(3, 1, mbf_light.GetAddressOf());
	m_pd3dImmediateContext->PSSetConstantBuffers(3, 1, mbf_light.GetAddressOf());
	m_pd3dImmediateContext->PSSetConstantBuffers(4, 1, mbf_cube.GetAddressOf());
	
}

void ObjectManager::InitSkyResource(ComPtr<ID3D11Device> m_pd3dDevice, ComPtr<ID3D11DeviceContext> m_pd3dImmediateContext)
{
	D3D11_BUFFER_DESC cbd;
	ZeroMemory(&cbd, sizeof(cbd));
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.ByteWidth = sizeof(WorldMatrixCB);
	HR(m_pd3dDevice->CreateBuffer(&cbd, nullptr, mbf_sky.GetAddressOf()));

	//天空 里面包含了天空缓冲区的更新
	//daylight.jpg
	m_skyCube.SetSkyCubeInfo(m_pd3dDevice.Get(), m_pd3dImmediateContext.Get(),
		L"Texture\\daylight.jpg", 5000.0f);

	m_pd3dImmediateContext->VSSetConstantBuffers(0, 1, mbf_sky.GetAddressOf());
}

void ObjectManager::SetObjRanderStates(ComPtr<ID3D11DeviceContext> m_pd3dImmediateContext)
{
	m_pd3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pd3dImmediateContext->IASetInputLayout(m_pVertexLayout3D.Get());
	m_pd3dImmediateContext->VSSetShader(m_pVertexShader3D.Get(), nullptr, 0);
	m_pd3dImmediateContext->PSSetShader(m_pPixelShader3D.Get(), nullptr, 0);
	m_pd3dImmediateContext->PSSetSamplers(0, 1, RenderStates::SSAnistropicWrap.GetAddressOf());
	//m_pd3dImmediateContext->RSSetState(nullptr);
	m_pd3dImmediateContext->RSSetState(RenderStates::RSNoCull.Get());
	m_pd3dImmediateContext->GSSetShader(nullptr, nullptr, 0);
	m_pd3dImmediateContext->OMSetDepthStencilState(nullptr, 0);
	m_pd3dImmediateContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
}

void ObjectManager::SetSkyRenderStates(ComPtr<ID3D11DeviceContext> m_pd3dImmediateContext) 
{
	m_pd3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pd3dImmediateContext->IASetInputLayout(m_SkyLayout3D.Get());
	m_pd3dImmediateContext->VSSetShader(m_SkyVertexShader3D.Get(), nullptr, 0);
	m_pd3dImmediateContext->PSSetShader(m_SkyPixelShader3D.Get(), nullptr, 0);
	m_pd3dImmediateContext->PSSetSamplers(0, 1, RenderStates::SSLinearWrap.GetAddressOf());
	//渲染状态
	m_pd3dImmediateContext->RSSetState(RenderStates::RSNoCull.Get());
	//深度模版状态
	m_pd3dImmediateContext->OMSetDepthStencilState(RenderStates::DSSLessEqual.Get(), 0);
	m_pd3dImmediateContext->GSSetShader(nullptr, nullptr, 0);
	m_pd3dImmediateContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
}

void ObjectManager::SetBoardRenderStates(ComPtr<ID3D11DeviceContext> m_pd3dImmediateContext)
{
	m_pd3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	m_pd3dImmediateContext->IASetInputLayout(m_BoardLayout3D.Get());
	m_pd3dImmediateContext->VSSetShader(m_BoardVertexShader3D.Get(), nullptr, 0);
	m_pd3dImmediateContext->PSSetShader(m_pPixelShader3D.Get(), nullptr, 0);
	m_pd3dImmediateContext->PSSetSamplers(0, 1, RenderStates::SSLinearWrap.GetAddressOf());
	//渲染状态
	m_pd3dImmediateContext->RSSetState(RenderStates::RSNoCull.Get());
	//深度模版状态
	m_pd3dImmediateContext->OMSetDepthStencilState(nullptr, 0);
	m_pd3dImmediateContext->GSSetShader(m_BoardGeometryShader3D.Get(), nullptr, 0);
	m_pd3dImmediateContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
}

void ObjectManager::SetChairRenderStates(ComPtr<ID3D11DeviceContext> m_pd3dImmediateContext)
{
	m_pd3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pd3dImmediateContext->IASetInputLayout(m_ChairLayout3D.Get());
	m_pd3dImmediateContext->VSSetShader(m_ChairVertexShader3D.Get(), nullptr, 0);
	m_pd3dImmediateContext->PSSetShader(m_ChairPixelShader3D.Get(), nullptr, 0);
	m_pd3dImmediateContext->RSSetState(RenderStates::RSNoCull.Get());
	m_pd3dImmediateContext->OMSetDepthStencilState(nullptr, 0);
	m_pd3dImmediateContext->GSSetShader(nullptr, nullptr, 0);

}

void ObjectManager::SetShadowRenderState(ComPtr<ID3D11DeviceContext> m_pd3dImmediateContext)
{
	m_pd3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pd3dImmediateContext->IASetInputLayout(m_pVertexLayout3D.Get());
	m_pd3dImmediateContext->VSSetShader(m_pVertexShader3D.Get(), nullptr, 0);
	m_pd3dImmediateContext->RSSetState(RenderStates::RSNoCull.Get());
	m_pd3dImmediateContext->PSSetShader(m_pPixelShader3D.Get(), nullptr, 0);
	m_pd3dImmediateContext->GSSetShader(nullptr, nullptr, 0);
	m_pd3dImmediateContext->PSSetSamplers(0, 1, RenderStates::SSLinearWrap.GetAddressOf());
	m_pd3dImmediateContext->OMSetDepthStencilState(RenderStates::DSSNoDoubleBlend.Get(), 0);
	m_pd3dImmediateContext->OMSetBlendState(RenderStates::BSTransparent.Get(), nullptr, 0xFFFFFFDD);
}

void ObjectManager::DrawObjIndexed(ComPtr<ID3D11DeviceContext> m_pd3dImmediateContext, GameObject obj)
{
	obj.DrawIndexed(m_pd3dImmediateContext.Get());
}

void ObjectManager::DrawObject(ComPtr<ID3D11DeviceContext> m_pd3dImmediateContext, GameObject obj)
{
	obj.Draw(m_pd3dImmediateContext.Get());
}

void ObjectManager::DrawSky(ComPtr<ID3D11DeviceContext> m_pd3dImmediateContext, const Camera& camera)
{
	m_skyCube.Draw(m_pd3dImmediateContext.Get(),camera);
}