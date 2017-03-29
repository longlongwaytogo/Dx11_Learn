#include "DxGraphics.h"
#include "Dx11Render.h"

CDxGraphics::CDxGraphics(void):m_pRender(NULL)
{
}


CDxGraphics::~CDxGraphics(void)
{
}

bool CDxGraphics::Initialize(int w,int h,HWND hwnd)
{
	m_pRender = new Dx11Render;
	if(!m_pRender)
		return false;

	// 调用D3DClass初始化函数 
	bool result = m_pRender->Initialize(w, h, hwnd,VSYNC_ENABLED, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR); 
	if(!result) 
	{ 
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK); 
		return false; 
	}
	return true;
}


void CDxGraphics::Shutdown()
{
	if(m_pRender)
	{
		m_pRender->Shutdown();
		delete m_pRender;
	}
	m_pRender = NULL;

}

bool CDxGraphics::Frame()
{
	bool result = false;

	//调用Render函数，渲染3D场景

	//Render是GraphicsClass的私有函数.

	result = Render(); 
	if(!result) 
	{ 
		return false; 
	}
	return true;
}

bool CDxGraphics::Render()
{
	
	// 设置framebuffer为浅蓝色 
	m_pRender->BeginScene(0.0f, 0.0f, 0.5f, 1.0f);


	//把framebuffer中的图像present到屏幕上.

	m_pRender->EndScene();

	return true;
}
