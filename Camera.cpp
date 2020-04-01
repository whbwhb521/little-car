#include "camera.h"
using namespace DirectX;
Camera::Camera()
	: m_Position(0.0f, 0.0f, 0.0f), m_Right(0.0f, 0.0f, 0.0f),
	m_Up(0.0f, 0.0f, 0.0f), m_Look(0.0f, 0.0f, 0.0f),
	m_NearZ(), m_FarZ(), m_FovY(), m_Aspect(),
	m_NearWindowHeight(), m_FarWindowHeight(),
	m_View(), m_Proj(), m_ViewPort(), CameraType(1)
{
}

Camera::~Camera() 
{
}

void Camera::SetViewPort(const D3D11_VIEWPORT& viewPort)
{
	m_ViewPort = viewPort;
}

void Camera::SetFrustum(float fovY, float aspect, float nearZ, float farZ)
{
	m_FovY = fovY;
	m_Aspect = aspect;
	m_NearZ = nearZ;
	m_FarZ = farZ;

	m_NearWindowHeight = 2.0f * m_NearZ * tanf(0.5f * m_FovY);
	m_FarWindowHeight = 2.0f * m_FarZ * tanf(0.5f * m_FovY);

	XMStoreFloat4x4(&m_Proj, XMMatrixPerspectiveFovLH(m_FovY, m_Aspect, m_NearZ, m_FarZ));
}

void Camera::SetViewPort(float topLeftX, float topLeftY, float width, float height,
	float minDepth, float maxDepth)
{
	D3D11_VIEWPORT viewPort;
	viewPort.TopLeftX = topLeftX;
	viewPort.TopLeftY = topLeftY;
	viewPort.Width = width;
	viewPort.Height = height;
	viewPort.MinDepth = minDepth;
	viewPort.MaxDepth = maxDepth;

	m_ViewPort = viewPort;
}

XMVECTOR Camera::GetPositionXM() const 
{
	return XMLoadFloat3(&m_Position);
}

XMFLOAT3 Camera::GetPosition() const
{
	return m_Position;
}

XMVECTOR Camera::GetRightXM() const 
{
	return XMLoadFloat3(&m_Right);
}

XMFLOAT3 Camera::GetRight() const 
{
	return m_Right;
}

XMVECTOR Camera::GetUpXM() const 
{
	return XMLoadFloat3(&m_Up);
}

XMFLOAT3 Camera::GetUp() const 
{
	return m_Up;
}

XMVECTOR Camera::GetLookXM() const 
{
	return XMLoadFloat3(&m_Look);
}

XMFLOAT4X4 Camera::GetView() const 
{
	return m_View;
}

XMMATRIX Camera::GetViewXM() const
{
	return XMLoadFloat4x4(&m_View);
}

XMFLOAT4X4 Camera::GetProj() const 
{
	return m_Proj;
}

XMMATRIX Camera::GetProjXM() const 
{
	return XMLoadFloat4x4(&m_Proj);
}

XMMATRIX Camera::GetViewProjXM() const 
{
	return XMLoadFloat4x4(&m_View) * XMLoadFloat4x4(&m_Proj);
}

UINT Camera::GetCameraType()
{
	return CameraType;
}

XMFLOAT3 Camera::GetLook() const
{
	return m_Look;
}

void Camera::SetCameraType(UINT type)
{
	Camera::CameraType = type;
}

//-----------------------第一人称相机信息-----------------------------------------------

FirstPersonCamera::FirstPersonCamera() : Camera()
{
}

FirstPersonCamera::~FirstPersonCamera() 
{
}

void FirstPersonCamera::SetPosition(float x, float y, float z) 
{
	SetPosition(XMFLOAT3(x, y, z));
}

void FirstPersonCamera::SetPosition(const XMFLOAT3 & v) 
{
	m_Position = v;
}

void FirstPersonCamera::LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up)
{
	LookAt(XMLoadFloat3(&pos), XMLoadFloat3(&target), XMLoadFloat3(&up));
}

void XM_CALLCONV FirstPersonCamera::LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR up)
{
	LookTo(pos, target-pos, up);
}

void FirstPersonCamera::LookTo(const XMFLOAT3& pos, const XMFLOAT3& to, const XMFLOAT3& up)
{
	LookTo(XMLoadFloat3(&pos), XMLoadFloat3(&to), XMLoadFloat3(&up));
}

void FirstPersonCamera::LookTo(FXMVECTOR pos, FXMVECTOR to, FXMVECTOR up)
{
	XMVECTOR L = XMVector3Normalize(to);  //看向的地方
	XMVECTOR R = XMVector3Normalize(XMVector3Cross(up, L)); //向右
	XMVECTOR U = XMVector3Cross(L, R);  //向上

	XMStoreFloat3(&m_Position, pos);
	XMStoreFloat3(&m_Look, L);
	XMStoreFloat3(&m_Right, R);
	XMStoreFloat3(&m_Up, U);
}

void FirstPersonCamera::LRWalk(float d) 
{
	XMVECTOR Pos = XMLoadFloat3(&m_Position);
	XMVECTOR Right = XMLoadFloat3(&m_Right);
	XMVECTOR Dist = XMVectorReplicate(d);
	XMStoreFloat3(&m_Position, XMVectorMultiplyAdd(Dist, Right, Pos));
}

void FirstPersonCamera::Walk(float d)
{
	XMVECTOR Pos = XMLoadFloat3(&m_Position);
	XMVECTOR Look = XMLoadFloat3(&m_Look);
	XMVECTOR Dist = XMVectorReplicate(d);
	// DestPos = Dist * Look + SrcPos
	XMStoreFloat3(&m_Position, XMVectorMultiplyAdd(Dist, Look, Pos));
}

void FirstPersonCamera::RotateRight(float rad)
{
	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&m_Right), rad);
	XMVECTOR Up = XMVector3TransformNormal(XMLoadFloat3(&m_Up), R);
	XMVECTOR Look = XMVector3TransformNormal(XMLoadFloat3(&m_Look), R);

	float cosPhi = XMVectorGetY(Look);
	// 将上下视野角度Phi限制在[2pi/9, 7pi/9]，
	// 即余弦值[-cos(2pi/9), cos(2pi/9)]之间
	if (fabs(cosPhi) > cosf(XM_2PI / 9))
		return;

	XMStoreFloat3(&m_Up, Up);
	XMStoreFloat3(&m_Look, Look);
}

void FirstPersonCamera::RotateY(float rad) 
{
	XMMATRIX R = XMMatrixRotationY(rad);
	XMVECTOR Right = XMVector3TransformNormal(XMLoadFloat3(&m_Right), R);
	XMVECTOR Up = XMVector3TransformNormal(XMLoadFloat3(&m_Up), R);
	XMVECTOR Look = XMVector3TransformNormal(XMLoadFloat3(&m_Look), R);
	XMStoreFloat3(&m_Right, Right);
	XMStoreFloat3(&m_Up, Up);
	XMStoreFloat3(&m_Look, Look);
}

void FirstPersonCamera::UpdateViewMatrix() 
{
	XMVECTOR R = XMLoadFloat3(&m_Right);
	XMVECTOR U = XMLoadFloat3(&m_Up);
	XMVECTOR L = XMLoadFloat3(&m_Look);
	XMVECTOR P = XMLoadFloat3(&m_Position);

	// 保持摄像机的轴互为正交，且长度都为1
	L = XMVector3Normalize(L);
	U = XMVector3Normalize(XMVector3Cross(L, R));

	// U, L已经正交化，需要计算对应叉乘得到R
	R = XMVector3Cross(U, L);

	XMStoreFloat3(&m_Right, R);
	XMStoreFloat3(&m_Up, U);
	XMStoreFloat3(&m_Look, L);

	float x = -XMVectorGetX(XMVector3Dot(P, R));
	float y = -XMVectorGetX(XMVector3Dot(P, U));
	float z = -XMVectorGetX(XMVector3Dot(P, L));
	m_View = {
		m_Right.x, m_Up.x, m_Look.x, 0.0f,
		m_Right.y, m_Up.y, m_Look.y, 0.0f,
		m_Right.z, m_Up.z, m_Look.z, 0.0f,
		x, y, z, 1.0f
	};
}

//--------------------------------------第三人称相机------------------------------------

ThirdPersonCamera::ThirdPersonCamera() :
	Camera(),m_Distance(), m_Theta(), 
	m_Phi(),m_MinDist(), m_MaxDist(), m_TargetPosition()
{
}

ThirdPersonCamera::~ThirdPersonCamera() 
{
}

XMFLOAT3 ThirdPersonCamera::GetTargetPosition() const
{
	return m_TargetPosition;
}

void ThirdPersonCamera::SetTargetPosition(const XMFLOAT3 & target) 
{
	m_TargetPosition = target;
}

float ThirdPersonCamera::GetDistance() const 
{
	return m_Distance;
}

void ThirdPersonCamera::SetDistance(float dist)
{
	m_Distance = dist;
}

void ThirdPersonCamera::SetMinMaxDistance(float minDist, float maxDist) 
{
	m_MinDist = minDist;
	m_MaxDist = maxDist;
}

void ThirdPersonCamera::ModifyDistance(float dist) 
{
	m_Distance += dist;
	if (m_Distance > m_MaxDist) m_Distance = m_MaxDist;
	if (m_Distance < m_MinDist) m_Distance = m_MinDist;
}

void ThirdPersonCamera::SetRotationX(float phi) 
{
	m_Phi = XMScalarModAngle(phi);
	if (m_Phi < XM_PI / 9) m_Phi = XM_PI / 9;
	if (m_Phi > XM_PIDIV2) m_Phi = XM_PIDIV2;
}

void ThirdPersonCamera::SetRotationY(float theta) 
{
	m_Theta = XMScalarModAngle(theta);
}

void ThirdPersonCamera::RotateX(float rad) 
{
	m_Phi -= rad;
	if (m_Phi < XM_PI / 9)
		m_Phi = XM_PI / 9;
	else if (m_Phi > XM_PIDIV2)
		m_Phi = XM_PIDIV2;
}

void ThirdPersonCamera::RotateY(float rad) 
{
	m_Theta = XMScalarModAngle(m_Theta - rad);
}

void ThirdPersonCamera::UpdateViewMatrix() 
{
	float x = m_TargetPosition.x - m_Distance * sinf(m_Phi) * sinf(m_Theta);
	float z = m_TargetPosition.z - m_Distance * sinf(m_Phi) * cosf(m_Theta);
	float y = m_TargetPosition.y + m_Distance * cosf(m_Phi);
	m_Position = XMFLOAT3(x, y, z);
	XMVECTOR P = XMLoadFloat3(&m_Position);
	//正则化指向目标的向量
	XMVECTOR L = XMVector3Normalize(XMLoadFloat3(&m_TargetPosition)-P);
	//右向量
	XMVECTOR R = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), L));
	//相机顶向量
	XMVECTOR U = XMVector3Cross(L, R);

	XMStoreFloat3(&m_Right, R);
	XMStoreFloat3(&m_Up, U);
	XMStoreFloat3(&m_Look, L);

	x = -XMVectorGetX(XMVector3Dot(P, R));
	y = -XMVectorGetX(XMVector3Dot(P, U));
	z = -XMVectorGetX(XMVector3Dot(P, L));

	m_View = {
		m_Right.x, m_Up.x, m_Look.x, 0.0f,
		m_Right.y, m_Up.y, m_Look.y, 0.0f,
		m_Right.z, m_Up.z, m_Look.z, 0.0f,
		x, y, z, 1.0f
	};
}


