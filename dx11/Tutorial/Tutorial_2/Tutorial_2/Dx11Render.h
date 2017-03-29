#pragma once
#include <d3d11.h>
#include <D3DX10math.h>
class Dx11Render
{
public:
	Dx11Render(void);
	~Dx11Render(void);

	bool Initialize(int screenWidth, int screenHeight, HWND hwnd,bool vsync, bool fullscreen,float screenDepth, float screenNear);
	void Frame();
	void BeginScene(float r, float g, float b, float a);
	void EndScene();
	void Shutdown();

private:
	bool m_vsync_enabled;
	int  m_videoCardMemory;
	char m_videoCardDescription[128];
	IDXGISwapChain*		m_pSwapChain;
	ID3D11Device*		m_pD3dDevice;
	ID3D11DeviceContext* m_pD3dDeviceContext;
	
	ID3D11RenderTargetView*     m_pRenderTargetView;
	
	ID3D11Texture2D*			m_pDepthBackBuf;

	ID3D11DepthStencilState*	m_pDepthStencilState;
	ID3D11DepthStencilView*     m_pDepthStencilView;
	ID3D11RasterizerState*		m_pRasterState;

	D3DXMATRIX					m_projectionMatrix;
	D3DXMATRIX					m_worldMatrix;
	D3DXMATRIX					m_orthoMatrix;

};

