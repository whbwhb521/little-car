#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <d3d11_1.h>
#include <DirectXMath.h>

class Camera
{
public:
	Camera();
	virtual ~Camera() = 0;
	//相机空间信息
	DirectX::XMVECTOR GetPositionXM() const;
	DirectX::XMFLOAT3 GetPosition() const;

	DirectX::XMVECTOR GetRightXM() const;
	DirectX::XMFLOAT3 GetRight() const;
	DirectX::XMVECTOR GetUpXM() const;
	DirectX::XMFLOAT3 GetUp() const;
	DirectX::XMVECTOR GetLookXM() const;
	DirectX::XMFLOAT3 GetLook() const;
	UINT GetCameraType();
	DirectX::XMFLOAT4X4 GetView() const;
	DirectX::XMMATRIX GetViewXM() const;
	DirectX::XMFLOAT4X4 GetProj() const;
	DirectX::XMMATRIX GetProjXM() const;
	DirectX::XMMATRIX GetViewProjXM() const;
	void SetCameraType(UINT type);

	//设置视锥体
	void SetFrustum(float fovY, float aspect, float nearZ, float farZ);
	//设置视口
	void SetViewPort(const D3D11_VIEWPORT& viewPort);
	void SetViewPort(float topLeftX, float topLeftY, float width, float height,
		float minDepth=0.0f, float maxDepth = 1.0f);

	// 更新观察矩阵
	virtual void UpdateViewMatrix() = 0;
protected:

	//相机属性
	DirectX::XMFLOAT3 m_Position;
	DirectX::XMFLOAT3 m_Right;
	DirectX::XMFLOAT3 m_Up;
	DirectX::XMFLOAT3 m_Look;

	float m_NearZ;
	float m_FarZ;
	float m_Aspect;
	float m_FovY;
	float m_NearWindowHeight;
	float m_FarWindowHeight;

	//观察矩阵和投影矩阵
	DirectX::XMFLOAT4X4 m_View;
	DirectX::XMFLOAT4X4 m_Proj;

	// 当前视口
	D3D11_VIEWPORT m_ViewPort;
	UINT CameraType;
};

class FirstPersonCamera : public Camera 
{
public:
	FirstPersonCamera();
	~FirstPersonCamera() override;
	//设置相机位置
	void SetPosition(float x, float y, float z);
	void SetPosition(const DirectX::XMFLOAT3& v);
	//设置相机朝向
	void XM_CALLCONV LookAt(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR target, DirectX::FXMVECTOR up);
	void LookAt(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up);
	void XM_CALLCONV LookTo(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR to, DirectX::FXMVECTOR up);
	void LookTo(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& to, const DirectX::XMFLOAT3& up);
	//左右平移
	void LRWalk(float d);
	//前后平移
	void Walk(float d);
	//上下观察
	void RotateRight(float rad);
	//左右观察
	void RotateY(float rad);

	void UpdateViewMatrix() override;

	
};

class ThirdPersonCamera : public Camera
{
public:
	ThirdPersonCamera();
	~ThirdPersonCamera() override;

	DirectX::XMFLOAT3 GetTargetPosition() const;
	//设置目标位置
	void SetTargetPosition(const DirectX::XMFLOAT3& target);
	//获取与目标的距离
	float GetDistance() const;
	//设置与目标的距离
	void SetDistance(float dist);
	//设置最大最小距离
	void SetMinMaxDistance(float minDist, float maxDist);
	//修改与目标的距离
	void ModifyDistance(float dist);
	// 设置初始绕X轴的弧度(注意上下视野角度Phi限制在[pi/6, pi/2])
	void SetRotationX(float phi);
	// 设置初始绕Y轴的弧度
	void SetRotationY(float theta);
	//绕X轴旋转
	void RotateX(float rad);
	//绕Y轴旋转
	void RotateY(float rad);

	void UpdateViewMatrix() override;

private:
	DirectX::XMFLOAT3 m_TargetPosition;

	float m_Distance, m_Theta, m_Phi, m_MinDist, m_MaxDist;
};







#endif // !CAMERA_H
