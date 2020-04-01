#pragma once
#ifndef GAME_H
#define GAME_H
#include "D3D_Window.h"
#include "MatrixCB.h"
#include "GameObject.h"
#include "SkyCube.h"
#include "camera.h"
#include "ObjectManager.h"
#include "RenderStates.h"
#include "RenderToTexture.h"

class Game : public D3DWindow 
{
public:
	Game(HINSTANCE hInstance);
	~Game();

	bool InitAll();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();
	void DrawAll();
	bool LC();


protected:
	bool InitEffect();
	bool InitResource();

private:
	ObjectManager objectManager;                //物体总管理器

	GameObject m_Car;
	GameObject m_Box;
	GameObject* m_Wheel;
	GameObject m_plane;
	GameObject m_Arm;
	GameObject m_Board;
	GameObject m_Chair;
	GameObject m_LeftHandle;
	GameObject m_RightHandle;
	GameObject m_Computer;
	GameObject m_ComputerBack;

	ComPtr<ID3D11Buffer> mbf_proj;                             //其次矩阵缓冲区
	ProjMatrixCB cb_proj;
	ComPtr<ID3D11Buffer> mbf_view;                             //观察矩阵的缓冲区
	ViewMatrixCB cb_View;
	ComPtr<ID3D11Buffer> mbf_isShadow;
	IsShadowCB isShadow;

	std::shared_ptr<Camera> m_pCamera;                         //摄像机
	std::shared_ptr<FirstPersonCamera> m1stCamera;
	std::shared_ptr<ThirdPersonCamera> m3rdCamera;

	RenderToTexture m_renderToTexture;
	ComPtr<ID3D11ShaderResourceView> ComputerTexture;

	const float R = 9 * sqrtf(2) * 0.5f / 8;
	 
};



#endif // !GAME_H