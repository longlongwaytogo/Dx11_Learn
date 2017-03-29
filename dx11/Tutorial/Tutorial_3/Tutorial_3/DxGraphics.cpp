#include "DxGraphics.h"

#include "Dx11Render.h"
#include "Camera.h"
#include "Model.h"
#include "ColorShader.h"

CDxGraphics::CDxGraphics(void):m_pRender(NULL),
	m_pCamera(NULL),m_pModel(NULL),m_pColorShader(NULL)
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
	m_pCamera = new CCamera;
	if(!m_pCamera)
		return false;

	// 设置摄像机位置.
	m_pCamera->SetPosition(0.0f, 0.0f, -10.0f);

	m_pColorShader = new CColorShader;
	if(!m_pColorShader)
		return false;
	// 初始化shader对象
	result = m_pColorShader->Initialize(m_pRender->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}


	m_pModel = new CModel;
	if(!m_pModel)
		return false;

	result = m_pModel->Initialize(m_pRender->GetDevice());
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
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

	if(m_pColorShader)
	{
		m_pColorShader->Shutdown();

		delete m_pColorShader;
	}
	if(m_pCamera)
	{
		delete m_pCamera;
	}
	if(m_pModel)
	{
	
		delete m_pModel;
	}
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
	/*
	  首先会计算三个矩阵：世界矩阵(模型坐标空间到世界坐标空间转化矩阵)、视点矩阵(世界坐标空间转化到视点坐标空间、或者说是摄像机坐标空间)
	  、投影矩阵(视点坐标空间进行投影操作，在透视投影情况下，产生一个称作frustum的视锥体，在平行投影的情况下，产生一个长方体)。
	*/
	
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix; 
	bool result;

	// 设置framebuffer为浅蓝色 
	m_pRender->BeginScene(0.0f, 0.0f, 0.5f, 1.0f);

	// 得到view矩阵. 
	m_pCamera->Render();

	// 得到3个矩阵. 
	viewMatrix = m_pCamera->GetViewMatrix(); 
	m_pRender->GetWorldMatrix(worldMatrix); 
	m_pRender->GetProjectionMatrix(projectionMatrix);

	// 把模型顶点和索引缓冲放入管线，准备渲染.
	m_pModel->RenderBuffers(m_pRender->GetDeviceContext());

	// 用shader渲染.
	result = m_pColorShader->Render(m_pRender->GetDeviceContext(), m_pModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if(!result)
	{
		return false;
	}

	//把framebuffer中的图像present到屏幕上.

	m_pRender->EndScene();

	return true;
}
