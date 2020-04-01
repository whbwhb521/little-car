#ifndef D3D_WINDOW_H
#define D3D_WINDOW_H

#include <wrl/client.h>
#include <string>
#include <d2d1.h>
#include <dwrite.h>
#include <d3d11_1.h>
#include <DirectXMath.h>
#include "Mouse.h"
#include "Keyboard.h"
#include "GameTimer.h"


// 添加所有要引用的库
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "winmm.lib")

class D3DWindow
{
public:
	D3DWindow(HINSTANCE hInstance);
	virtual ~D3DWindow();
	float AspectRatio() const;
	HWND      MainWnd()const;       // 获取主窗口句柄
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	// 窗口的消息回调函数
	virtual void OnResize();                  // 该父类方法需要在窗口大小变动的时候调用

protected:
	bool InitWindow();
	bool InitD3d();
	void CalculateFrameStats(); // 计算每秒帧数并在窗口显示


	HINSTANCE m_hAppInst;        // 应用实例句柄
	HWND      m_hMainWnd;        // 主窗口句柄
	std::wstring m_MainWndCaption;                        // 主窗口标题
	int m_ClientWidth;           // 视口宽度
	int m_ClientHeight;          // 视口高度

	bool      m_AppPaused;       // 应用是否暂停
	bool      m_Minimized;       // 应用是否最小化
	bool      m_Maximized;       // 应用是否最大化
	bool      m_Resizing;        // 窗口大小是否变化
	bool	  m_Enable4xMsaa;	 // 是否开启4倍多重采样
	UINT      m_4xMsaaQuality;   // MSAA支持的质量等级
	GameTimer m_Timer;           // 计时器


	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	ComPtr<ID3D11Device> m_pd3dDevice;
	ComPtr<ID3D11DeviceContext> m_pd3dImmediateContext;			// D3D11设备上下文
	ComPtr<IDXGISwapChain> m_pSwapChain;						// D3D11交换链
	// Direct3D 11.1
	ComPtr<ID3D11Device1> m_pd3dDevice1;						// D3D11.1设备
	ComPtr<ID3D11DeviceContext1> m_pd3dImmediateContext1;		// D3D11.1设备上下文
	ComPtr<IDXGISwapChain1> m_pSwapChain1;						// D3D11.1交换链
	// 常用资源
	ComPtr<ID3D11Texture2D> m_pDepthStencilBuffer;				// 深度模板缓冲区
	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;			// 渲染目标视图
	ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;			// 深度模板视图
	D3D11_VIEWPORT m_ScreenViewport;                            // 视口
	// 键鼠输入
	std::unique_ptr<DirectX::Mouse> m_pMouse;					// 鼠标
	DirectX::Mouse::ButtonStateTracker m_MouseTracker;			// 鼠标状态追踪器
	std::unique_ptr<DirectX::Keyboard> m_pKeyboard;				// 键盘
	DirectX::Keyboard::KeyboardStateTracker m_KeyboardTracker;	// 键盘状态追踪器
};

#endif // !D3D_WINDOW_H