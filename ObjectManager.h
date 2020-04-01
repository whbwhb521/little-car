#pragma once
#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H
#include <d3d11_1.h>
#include <DirectXMath.h>
#include <d2d1.h>
#include <wrl/client.h>
#include "SomeModel.h"
#include "MatrixCB.h"
#include "DXTrace.h"
#include "GameObject.h"
#include "SkyCube.h"

class ObjectManager 
{
public:
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	ObjectManager();
	~ObjectManager();
	//获取小车
	GameObject GetBox();
	GameObject* GetWheel();
	GameObject GetPlane();
	GameObject GetArm();
	GameObject GetBoard();
	GameObject GetChair();
	GameObject GetLeftHandle();
	GameObject GetRightHandle();
	GameObject GetMyComputer();
	GameObject GetMyComputerBack();
	void InitShaderAndLayout(ComPtr<ID3D11Device> m_pd3dDevice);                    //初始化布局和着色器
	 //初始化物体的一系列资源
	void InitObjResource(ComPtr<ID3D11Device> m_pd3dDevice, ComPtr<ID3D11DeviceContext> m_pd3dImmediateContext);     
	//初始化天空的一系列资源
	void InitSkyResource(ComPtr<ID3D11Device> m_pd3dDevice, ComPtr<ID3D11DeviceContext> m_pd3dImmediateContext);    

	void SetObjRanderStates(ComPtr<ID3D11DeviceContext> m_pd3dImmediateContext);    //设置物体的一些rander
	void SetSkyRenderStates(ComPtr<ID3D11DeviceContext> m_pd3dImmediateContext);    //设置天空的一些rander
	void SetBoardRenderStates(ComPtr<ID3D11DeviceContext> m_pd3dImmediateContext);  //设置木板的一些render
	void SetChairRenderStates(ComPtr<ID3D11DeviceContext> m_pd3dImmediateContext);  //设置座位和操纵杆的一些render
	void SetShadowRenderState(ComPtr<ID3D11DeviceContext> m_pd3dImmediateContext);

	void DrawObject(ComPtr<ID3D11DeviceContext> m_pd3dImmediateContext, GameObject obj);
	void DrawObjIndexed(ComPtr<ID3D11DeviceContext> m_pd3dImmediateContext, GameObject obj);
	void DrawSky(ComPtr<ID3D11DeviceContext> m_pd3dImmediateContext, const Camera& camera);

private:
	GameObject m_Box;
	GameObject m_Arm;
	GameObject m_Wheel[4];
	GameObject m_plane;
	SkyCube m_skyCube;
	GameObject m_Board;
	GameObject m_Chair;
	GameObject m_LeftHandle;
	GameObject m_RightHandle;
	GameObject m_Computer;
	GameObject m_Computer_back;

	//对于物体而言的缓冲区
	ComPtr<ID3D11Buffer> mbf_world;                            //世界矩阵缓冲区
	ComPtr<ID3D11Buffer> mbf_light;                            //灯光的缓冲区
	ComPtr<ID3D11Buffer> mbf_cube;

	//对应的矩阵
	LightMatrixCB cb_light;

	//对于天空而言的缓冲区
	ComPtr<ID3D11Buffer> mbf_sky;                              //天空盒子缓冲区

	//物体的着色器和布局
	ComPtr<ID3D11InputLayout> m_pVertexLayout3D;				// 用于3D的顶点输入布局
	ComPtr<ID3D11VertexShader> m_pVertexShader3D;				// 用于3D的顶点着色器
	ComPtr<ID3D11PixelShader> m_pPixelShader3D;				    // 用于3D的像素着色器

	//天空的着色器和布局
	ComPtr<ID3D11InputLayout> m_SkyLayout3D;                    //天空布局
	ComPtr<ID3D11VertexShader> m_SkyVertexShader3D;				// 用于3D的顶点着色器
	ComPtr<ID3D11PixelShader> m_SkyPixelShader3D;				// 用于3D的像素着色器

	//座位和操纵杆的着色器和布局 
	ComPtr<ID3D11InputLayout> m_ChairLayout3D;
	ComPtr<ID3D11VertexShader> m_ChairVertexShader3D;
	ComPtr<ID3D11PixelShader> m_ChairPixelShader3D;

	//木板布局和着色器
	ComPtr<ID3D11GeometryShader> m_BoardGeometryShader3D;       //木板的3D几何着色器
	ComPtr<ID3D11VertexShader> m_BoardVertexShader3D;
	ComPtr<ID3D11InputLayout> m_BoardLayout3D;                    //天空布局

	ComPtr<ID3D11ShaderResourceView> Objtexture;
	const float boxheight = 0.5f;
	const float wheelraudis = 0.25f;

};


#endif // !OBJECTMANAGER_H
