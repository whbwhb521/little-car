#include "GameThings.h"
#include "d3dUtil.h"
#include "DXTrace.h"
#include "Vertex.h"
#include "SomeModel.h"
#include "GameObject.h"

using namespace DirectX;

Game::Game(HINSTANCE hInstance)
	:D3DWindow(hInstance),
	cb_proj()
{
	m1stCamera = std::shared_ptr<FirstPersonCamera>(new FirstPersonCamera);
	m3rdCamera = std::shared_ptr<ThirdPersonCamera>(new ThirdPersonCamera);
}

Game::~Game() {}

bool Game::InitAll() 
{
	if (!D3DWindow::InitWindow()) return false;
	if (!D3DWindow::InitD3d()) return false;
	if (!InitEffect()) return false;
	if (!InitResource()) return false;
	return true;
}

void Game::OnResize()
{
	D3DWindow::OnResize();
}

void Game::UpdateScene(float dt) 
{
	// 更新鼠标事件，获取相对偏移量
	Mouse::State mouseState = m_pMouse->GetState();
	Mouse::State lastMouseState = m_MouseTracker.GetLastState();

	Keyboard::State keyState = m_pKeyboard->GetState();
	m_KeyboardTracker.Update(keyState);

	//CameraType {1:第一人称 ， 2:第一人称+自由视角 ， 3:第三人称}
	static float Allwalk = 0.0f;
	float walk = 0.0f;
	static float angle = 0.0f;
	float wheelangle = 0.0f;
	float LHandleangle = 0.0f;
	float RHandleangle = 0.0f;
	if (keyState.IsKeyDown(Keyboard::W))
	{
		LHandleangle = XM_PI / 6.0f;
		walk = dt * 3.0f;
		if (m_pCamera->GetCameraType() == 1)
		{
		}
		else if (m_pCamera->GetCameraType() == 2)
		{
			m1stCamera->Walk(walk);
		}
		else
		{
		}
	}

	if (keyState.IsKeyDown(Keyboard::S)) 
	{	
		LHandleangle = -XM_PI / 6.0f;
		walk = dt * -3.0f;
		if (m_pCamera->GetCameraType() == 1)
		{
			
		}
		else if (m_pCamera->GetCameraType() == 2)
		{
			m1stCamera->Walk(walk);
		}
		else
		{
		}
	}

	if (keyState.IsKeyDown(Keyboard::A))
	{
		RHandleangle = -XM_PI / 6.0f;
		
		if (m_pCamera->GetCameraType() == 1)
		{
			wheelangle = -XM_PIDIV4 + angle;
			m1stCamera->RotateY(-walk * 0.2f);
			angle -= walk * 0.2f;
		}
		else if (m_pCamera->GetCameraType() == 2)
		{
			m1stCamera->LRWalk(dt * -3.0f);
		}
		else
		{
			wheelangle = -XM_PIDIV4 + angle;
			angle -= walk * 0.2f;
		}
	}

	if (keyState.IsKeyDown(Keyboard::D))
	{
		RHandleangle = XM_PI / 6.0f;
		
		if (m_pCamera->GetCameraType() == 1)
		{
			wheelangle = XM_PIDIV4 + angle;
			m1stCamera->RotateY(walk * 0.2f);
			angle += walk * 0.2f;
			
		}
		else if (m_pCamera->GetCameraType() == 2)
		{
			m1stCamera->LRWalk(dt * 3.0f);
		}
		else
		{
			wheelangle = XM_PIDIV4 + angle;
			angle += walk * 0.2f;
		}
	}
	if (wheelangle != angle && (keyState.IsKeyUp(Keyboard::D) && keyState.IsKeyUp(Keyboard::A) || 
		keyState.IsKeyDown(Keyboard::D) && keyState.IsKeyDown(Keyboard::A)))
	{
		wheelangle = angle;
		RHandleangle = 0.0f;
	}

	Allwalk += walk;
	if (m_pCamera->GetCameraType() == 1)
	{
		//小车和轮子的移动以及轮子的自转
		m_Box.SetWorldMatrix(XMMatrixMultiply(XMMatrixRotationY(angle),
			XMMatrixTranslation(m_Box.GetPosition().x + walk * sinf(angle),
				m_Box.GetPosition().y, m_Box.GetPosition().z + walk * cosf(angle))));
		float rotatangle;
		for (int i = 0; i < 4; i++)
		{
			if (i == 0 || i == 3) rotatangle = XM_PIDIV4 + angle;
			else rotatangle = XM_PIDIV4 - angle;
			m_Wheel[i].SetWorldMatrix(XMMatrixMultiply(XMMatrixRotationY(i < 2 ? wheelangle : angle),
				XMMatrixTranslation(i % 2 == 0 ? m_Box.GetPosition().x - R * cosf(rotatangle) : m_Box.GetPosition().x + R * cosf(rotatangle),
					m_Wheel[i].GetPosition().y,
					i < 2 ? m_Box.GetPosition().z + R * sinf(rotatangle) : m_Box.GetPosition().z - R * sinf(rotatangle))));
			//自转速度为 Degree = walk / 2R;这里算出来为 2*walk
			m_Wheel[i].SetWorldMatrix(XMMatrixMultiply(XMMatrixRotationX(2 * Allwalk), XMLoadFloat4x4(&m_Wheel[i].GetWorldMatrix())));
		}
		rotatangle = 3.0f * XM_PI / 8.0f - angle;
		m_Arm.SetWorldMatrix(XMMatrixMultiply(XMMatrixRotationY(angle), XMMatrixTranslation(m_Box.GetPosition().x + 1.0f * cosf(rotatangle),
			m_Box.GetPosition().y + 0.4f,
			m_Box.GetPosition().z + 1.0f * sinf(rotatangle))));
		//相机跟随小车
		m1stCamera->SetPosition(m_Box.GetPosition().x - 3.0f * sinf(angle), m_Box.GetPosition().y + 3.0f, m_Box.GetPosition().z - 3.0f * cosf(angle));
		m_Chair.SetWorldMatrix(XMMatrixMultiply(XMMatrixRotationY(angle), XMMatrixTranslation(m_Box.GetPosition().x, 0.7525f, m_Box.GetPosition().z)));
		rotatangle = XM_PIDIV4 + angle;
		XMMATRIX XZ = XMMatrixRotationX(LHandleangle) * XMMatrixRotationY(angle);
		m_LeftHandle.SetWorldMatrix(XMMatrixMultiply(XZ, XMMatrixTranslation(m_Box.GetPosition().x - 0.15f * cosf(rotatangle),
			0.75f,
			m_Box.GetPosition().z + 0.15f * sinf(rotatangle))));
		rotatangle = XM_PIDIV4 - angle;
		XMMATRIX YZ = XMMatrixRotationZ(-RHandleangle) * XMMatrixRotationY(angle);
		m_RightHandle.SetWorldMatrix(XMMatrixMultiply(YZ, XMMatrixTranslation(m_Box.GetPosition().x + 0.15f * cosf(rotatangle),
			0.75f,
			m_Box.GetPosition().z + 0.15f * sinf(rotatangle))));
		m_Computer.SetWorldMatrix(XMMatrixMultiply(XMMatrixMultiply(XMMatrixRotationX(-XM_PIDIV2), XMMatrixRotationY(angle)),
			XMMatrixTranslation(m_Box.GetPosition().x + 0.4f * sinf(angle),
				1.1f, m_Box.GetPosition().z + 0.4f * cosf(angle))));
		m_ComputerBack.SetWorldMatrix(XMMatrixMultiply(XMMatrixRotationY(angle),
			XMMatrixTranslation(m_Box.GetPosition().x + 0.426f * sinf(angle),
				1.1f, m_Box.GetPosition().z + 0.426f * cosf(angle))));
	}

	if (m_pCamera->GetCameraType() == 2)
	{//小车不需要移动，控制鼠标的移动即可
		m1stCamera->RotateRight(mouseState.y * dt * 1.25f);
		m1stCamera->RotateY(mouseState.x * dt * 1.25f);
	}
	if (m_pCamera->GetCameraType() == 3) 
	{
		m3rdCamera->SetTargetPosition(m_Box.GetPosition());
		m3rdCamera->RotateX(mouseState.y * dt * 1.25f);
		m3rdCamera->RotateY(mouseState.x * dt * 1.25f);
		m3rdCamera->ModifyDistance(-mouseState.scrollWheelValue / 120 * 1.0f);

		//小车和轮子的移动以及轮子的自转
		float rotatangle;
		for (int i = 0; i < 4; i++) 
		{
			if (i == 0 || i == 3) rotatangle = XM_PIDIV4 + angle;
			else rotatangle = XM_PIDIV4 - angle;
			m_Wheel[i].SetWorldMatrix(XMMatrixMultiply(XMMatrixRotationY(i < 2 ? wheelangle : angle),
				XMMatrixTranslation(i % 2 == 0 ? m_Box.GetPosition().x - R * cosf(rotatangle) : m_Box.GetPosition().x + R * cosf(rotatangle),
					m_Wheel[i].GetPosition().y,
					i < 2 ? m_Box.GetPosition().z + R * sinf(rotatangle) : m_Box.GetPosition().z - R * sinf(rotatangle))));
			//自转速度为 Degree = walk / 2R;这里算出来为 2*walk
			m_Wheel[i].SetWorldMatrix(XMMatrixMultiply(XMMatrixRotationX(2 * Allwalk), XMLoadFloat4x4(&m_Wheel[i].GetWorldMatrix())));
		}

		m_Box.SetWorldMatrix(XMMatrixMultiply(XMMatrixRotationY(angle),
			XMMatrixTranslation(m_Box.GetPosition().x + walk * sinf(angle),
				m_Box.GetPosition().y, m_Box.GetPosition().z + walk * cosf(angle))));
		rotatangle = 3.0f * XM_PI / 8.0f - angle;
		m_Arm.SetWorldMatrix(XMMatrixMultiply(XMMatrixRotationY(angle), XMMatrixTranslation(m_Box.GetPosition().x + 1.0f * cosf(rotatangle),
			m_Box.GetPosition().y + 0.4f, 
			m_Box.GetPosition().z + 1.0f * sinf(rotatangle))));
		m_Chair.SetWorldMatrix(XMMatrixMultiply(XMMatrixRotationY(angle), XMMatrixTranslation(m_Box.GetPosition().x, 0.7525f, m_Box.GetPosition().z)));
		rotatangle = XM_PIDIV4 + angle;
		XMMATRIX XZ = XMMatrixRotationX(LHandleangle) * XMMatrixRotationY(angle);
		m_LeftHandle.SetWorldMatrix(XMMatrixMultiply(XZ, XMMatrixTranslation(m_Box.GetPosition().x - 0.15f * cosf(rotatangle),
			0.75f,
			m_Box.GetPosition().z + 0.15f * sinf(rotatangle))));
		rotatangle = XM_PIDIV4 - angle;
		XMMATRIX YZ = XMMatrixRotationZ(-RHandleangle) * XMMatrixRotationY(angle);
		m_RightHandle.SetWorldMatrix(XMMatrixMultiply(YZ, XMMatrixTranslation(m_Box.GetPosition().x + 0.15f * cosf(rotatangle),
			0.75f,
			m_Box.GetPosition().z + 0.15f * sinf(rotatangle))));
		m_Computer.SetWorldMatrix(XMMatrixMultiply(XMMatrixMultiply(XMMatrixRotationX(-XM_PIDIV2), XMMatrixRotationY(angle)),
			XMMatrixTranslation(m_Box.GetPosition().x + 0.4f * sinf(angle),
				1.1f, m_Box.GetPosition().z + 0.4f * cosf(angle))));
		m_ComputerBack.SetWorldMatrix(XMMatrixMultiply(XMMatrixRotationY(angle),
			XMMatrixTranslation(m_Box.GetPosition().x + 0.426f * sinf(angle),
				1.1f, m_Box.GetPosition().z + 0.426f * cosf(angle))));
	}

	//换到第一人称视角
	if (m_KeyboardTracker.IsKeyPressed(Keyboard::D1) && m_pCamera->GetCameraType() != 1)
	{
		m1stCamera.reset(new FirstPersonCamera);
		m1stCamera->SetFrustum(XM_PI / 3, AspectRatio(), 0.5f, 1000.0f);
		XMFLOAT3 pos = XMFLOAT3(m_Box.GetPosition().x - 3.0f * sinf(angle), m_Box.GetPosition().y + 3.0f, m_Box.GetPosition().z - 3.0f * cosf(angle));
		XMFLOAT3 to = XMFLOAT3(m_Box.GetPosition().x + 5.0f * sinf(angle), m_Box.GetPosition().y, m_Box.GetPosition().z + 5.0f * cosf(angle));
		XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f);
		m1stCamera->LookAt(pos, to, up);
		m1stCamera->SetCameraType(1);
		m_pCamera = m1stCamera;
	}
	//第一人称自由视角
	if (m_KeyboardTracker.IsKeyPressed(Keyboard::D2) && m_pCamera->GetCameraType() != 2)
	{
		m1stCamera.reset(new FirstPersonCamera);
		m1stCamera->SetFrustum(XM_PI / 3, AspectRatio(), 0.5f, 1000.0f);
		XMFLOAT3 pos = m_Box.GetPosition();
		XMFLOAT3 to = XMFLOAT3(0.0f, 0.0f, 1.0f);
		XMStoreFloat3(&to, XMVector3TransformNormal(XMLoadFloat3(&to), XMMatrixRotationY(angle)));
		XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f);
		pos.y += 3;
		m1stCamera->LookTo(pos, to, up);
		m1stCamera->SetCameraType(2);
		m_pCamera = m1stCamera;
	}
	//第三人称跟随物体视角
	if (m_KeyboardTracker.IsKeyPressed(Keyboard::D3) && m_pCamera->GetCameraType() != 3)
	{
		m3rdCamera.reset(new ThirdPersonCamera);
		m3rdCamera->SetFrustum(XM_PI / 3, AspectRatio(), 0.5f, 1000.0f);
		m3rdCamera->SetRotationY(angle);
		m3rdCamera->SetTargetPosition(m_Box.GetPosition());
		m3rdCamera->SetDistance(4.0f);
		m3rdCamera->SetMinMaxDistance(2.0f, 8.0f);
		m3rdCamera->SetCameraType(3);
		m_pCamera = m3rdCamera;
	}

	// 退出程序，这里应向窗口发送销毁信息
	if (keyState.IsKeyDown(Keyboard::Escape))
		SendMessage(MainWnd(), WM_DESTROY, 0, 0);
	
	m_pMouse->ResetScrollWheelValue();
	m_pCamera->UpdateViewMatrix();
	DirectX::XMStoreFloat4(&cb_View.eyePos, m_pCamera->GetPositionXM());
	cb_View.view = XMMatrixTranspose(m_pCamera->GetViewXM());

	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(m_pd3dImmediateContext->Map(mbf_view.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	memcpy_s(mappedData.pData, sizeof(ViewMatrixCB), &cb_View, sizeof(ViewMatrixCB));
	m_pd3dImmediateContext->Unmap(mbf_view.Get(), 0);
}

void Game::DrawScene() 
{
	assert(m_pd3dImmediateContext);
	assert(m_pSwapChain);

	static float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };	// RGBA = (0,0,0,255)
	m_pd3dImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(), black);
	m_pd3dImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//绘制小电脑
	//RTT获取当前视角下的纹理赋值给电脑。
	m_renderToTexture.StartRender(m_pd3dImmediateContext.Get());
	DrawAll();
	m_renderToTexture.EndRender(m_pd3dImmediateContext.Get());
	ComputerTexture = m_renderToTexture.GetOutputTexture();
	m_Computer.SetObjTexture(ComputerTexture.Get(), false);
	objectManager.SetObjRanderStates(m_pd3dImmediateContext.Get());
	objectManager.DrawObjIndexed(m_pd3dImmediateContext.Get(), m_Computer);
	DrawAll();
	//设置木板状态
	objectManager.SetBoardRenderStates(m_pd3dImmediateContext.Get());
	objectManager.DrawObject(m_pd3dImmediateContext.Get(), m_Board);
	

	HR(m_pSwapChain->Present(0, 0));
}

void Game::DrawAll()
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	//绘制阴影
	isShadow.isShadow = true;
	isShadow.material.ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	isShadow.material.diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.5f);
	isShadow.material.specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 16.0f);
	HR(m_pd3dImmediateContext->Map(mbf_isShadow.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	memcpy_s(mappedData.pData, sizeof(IsShadowCB), &isShadow, sizeof(IsShadowCB));
	m_pd3dImmediateContext->Unmap(mbf_isShadow.Get(), 0);
	objectManager.SetShadowRenderState(m_pd3dImmediateContext.Get());
	objectManager.DrawObjIndexed(m_pd3dImmediateContext.Get(), m_Box);
	objectManager.DrawObjIndexed(m_pd3dImmediateContext.Get(), m_Arm);
	objectManager.DrawObjIndexed(m_pd3dImmediateContext.Get(), m_Wheel[0]);
	objectManager.DrawObjIndexed(m_pd3dImmediateContext.Get(), m_Wheel[1]);
	objectManager.DrawObjIndexed(m_pd3dImmediateContext.Get(), m_Wheel[2]);
	objectManager.DrawObjIndexed(m_pd3dImmediateContext.Get(), m_Wheel[3]);
	objectManager.DrawObjIndexed(m_pd3dImmediateContext.Get(), m_Chair);
	objectManager.DrawObjIndexed(m_pd3dImmediateContext.Get(), m_LeftHandle);
	objectManager.DrawObjIndexed(m_pd3dImmediateContext.Get(), m_RightHandle);
	//关闭阴影
	isShadow.isShadow = false;
	isShadow.material.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	isShadow.material.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f);
	isShadow.material.specular = XMFLOAT4(0.4f, 0.4f, 0.4f, 16.0f);
	HR(m_pd3dImmediateContext->Map(mbf_isShadow.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	memcpy_s(mappedData.pData, sizeof(IsShadowCB), &isShadow, sizeof(IsShadowCB));
	m_pd3dImmediateContext->Unmap(mbf_isShadow.Get(), 0);

	//设置物体的状态
	objectManager.SetObjRanderStates(m_pd3dImmediateContext.Get());
	objectManager.DrawObjIndexed(m_pd3dImmediateContext.Get(), m_Box);
	objectManager.DrawObjIndexed(m_pd3dImmediateContext.Get(), m_Arm);
	objectManager.DrawObjIndexed(m_pd3dImmediateContext.Get(), m_Wheel[0]);
	objectManager.DrawObjIndexed(m_pd3dImmediateContext.Get(), m_Wheel[1]);
	objectManager.DrawObjIndexed(m_pd3dImmediateContext.Get(), m_Wheel[2]);
	objectManager.DrawObjIndexed(m_pd3dImmediateContext.Get(), m_Wheel[3]);
	objectManager.DrawObjIndexed(m_pd3dImmediateContext.Get(), m_plane);
	//椅子状态，只有颜色没有纹理
	objectManager.SetChairRenderStates(m_pd3dImmediateContext.Get());
	objectManager.DrawObjIndexed(m_pd3dImmediateContext.Get(), m_Chair);
	objectManager.DrawObjIndexed(m_pd3dImmediateContext.Get(), m_LeftHandle);
	objectManager.DrawObjIndexed(m_pd3dImmediateContext.Get(), m_RightHandle);
	objectManager.DrawObjIndexed(m_pd3dImmediateContext.Get(), m_ComputerBack);

	//设置天空的状态
	objectManager.SetSkyRenderStates(m_pd3dImmediateContext.Get());
	//绘制天空
	objectManager.DrawSky(m_pd3dImmediateContext.Get(), *m_pCamera);
}

bool Game::InitEffect() 
{
	RenderStates::InitAll(m_pd3dDevice.Get());
	objectManager.InitShaderAndLayout(m_pd3dDevice);
	return true;
}

bool Game::InitResource() 
{  
	//创建裁剪空间和视角空间的缓冲区
	D3D11_BUFFER_DESC cbd;
	ZeroMemory(&cbd, sizeof(cbd));
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.ByteWidth = sizeof(ProjMatrixCB);
	HR(m_pd3dDevice->CreateBuffer(&cbd, nullptr, mbf_proj.GetAddressOf()));
	cbd.ByteWidth = sizeof(ViewMatrixCB);
	HR(m_pd3dDevice->CreateBuffer(&cbd, nullptr, mbf_view.GetAddressOf()));
	cbd.ByteWidth = sizeof(IsShadowCB);
	HR(m_pd3dDevice->CreateBuffer(&cbd, nullptr, mbf_isShadow.GetAddressOf()));

	//初始化物体和天空资源
	objectManager.InitObjResource(m_pd3dDevice.Get(), m_pd3dImmediateContext.Get());
	objectManager.InitSkyResource(m_pd3dDevice.Get(), m_pd3dImmediateContext.Get());
	//获取小车包括轮子
	m_Box = objectManager.GetBox();
	m_Wheel = objectManager.GetWheel();
	m_plane = objectManager.GetPlane();
	m_Arm = objectManager.GetArm();
	m_Board = objectManager.GetBoard();
	m_Chair = objectManager.GetChair();
	m_LeftHandle = objectManager.GetLeftHandle();
	m_RightHandle = objectManager.GetRightHandle();
	m_Computer = objectManager.GetMyComputer();
	m_ComputerBack = objectManager.GetMyComputerBack();

	//新建相机
	m_pCamera = m1stCamera;
	m1stCamera->SetViewPort(0.0f, 0.0f, (float)m_ClientWidth, (float)m_ClientHeight);
	m1stCamera->LookAt(XMFLOAT3(m_Box.GetPosition().x, m_Box.GetPosition().y + 3.0f, m_Box.GetPosition().z - 3.0f),
		XMFLOAT3(0.0f, m_Box.GetPosition().y, 5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));
	m_pCamera->SetFrustum(XM_PI / 3, AspectRatio(), 0.5f, 1000.0f);
	//设置裁剪空间
	cb_proj.proj = XMMatrixTranspose(m_pCamera->GetProjXM());
	m_renderToTexture.InitResource(m_pd3dDevice.Get(), 400, 400);

	//更新缓冲区
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(m_pd3dImmediateContext->Map(mbf_proj.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0,
		&mappedData));
	memcpy_s(mappedData.pData, sizeof(ProjMatrixCB), &cb_proj, sizeof(ProjMatrixCB));
	m_pd3dImmediateContext->Unmap(mbf_proj.Get(), 0);

	
	m_pd3dImmediateContext->VSSetConstantBuffers(2, 1, mbf_proj.GetAddressOf());
	m_pd3dImmediateContext->VSSetConstantBuffers(1, 1, mbf_view.GetAddressOf());
	m_pd3dImmediateContext->PSSetConstantBuffers(1, 1, mbf_view.GetAddressOf());
	m_pd3dImmediateContext->GSSetConstantBuffers(1, 1, mbf_view.GetAddressOf());
	m_pd3dImmediateContext->GSSetConstantBuffers(2, 1, mbf_proj.GetAddressOf());

	m_pd3dImmediateContext->VSSetConstantBuffers(5, 1, mbf_isShadow.GetAddressOf());
	m_pd3dImmediateContext->PSSetConstantBuffers(5, 1, mbf_isShadow.GetAddressOf());

	//鼠标键盘
	m_pMouse->SetWindow(m_hMainWnd);
	m_pMouse->SetMode(DirectX::Mouse::MODE_RELATIVE);

	return true;
}

bool Game::LC()
{
	MSG msg = { 0 };

	m_Timer.Reset();

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			m_Timer.Tick();

			if (!m_AppPaused)
			{
				CalculateFrameStats();
				UpdateScene(m_Timer.DeltaTime());
				DrawScene();
			}
			else
			{
				Sleep(100);
			}
		}
	}

	return (int)msg.wParam;
}
