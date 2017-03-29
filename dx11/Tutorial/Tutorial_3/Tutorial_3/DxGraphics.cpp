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

	// ����D3DClass��ʼ������ 
	bool result = m_pRender->Initialize(w, h, hwnd,VSYNC_ENABLED, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR); 
	if(!result) 
	{ 
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK); 
		return false; 
	}
	m_pCamera = new CCamera;
	if(!m_pCamera)
		return false;

	// ���������λ��.
	m_pCamera->SetPosition(0.0f, 0.0f, -10.0f);

	m_pColorShader = new CColorShader;
	if(!m_pColorShader)
		return false;
	// ��ʼ��shader����
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

	//����Render��������Ⱦ3D����

	//Render��GraphicsClass��˽�к���.

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
	  ���Ȼ�������������������(ģ������ռ䵽��������ռ�ת������)���ӵ����(��������ռ�ת�����ӵ�����ռ䡢����˵�����������ռ�)
	  ��ͶӰ����(�ӵ�����ռ����ͶӰ��������͸��ͶӰ����£�����һ������frustum����׶�壬��ƽ��ͶӰ������£�����һ��������)��
	*/
	
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix; 
	bool result;

	// ����framebufferΪǳ��ɫ 
	m_pRender->BeginScene(0.0f, 0.0f, 0.5f, 1.0f);

	// �õ�view����. 
	m_pCamera->Render();

	// �õ�3������. 
	viewMatrix = m_pCamera->GetViewMatrix(); 
	m_pRender->GetWorldMatrix(worldMatrix); 
	m_pRender->GetProjectionMatrix(projectionMatrix);

	// ��ģ�Ͷ�����������������ߣ�׼����Ⱦ.
	m_pModel->RenderBuffers(m_pRender->GetDeviceContext());

	// ��shader��Ⱦ.
	result = m_pColorShader->Render(m_pRender->GetDeviceContext(), m_pModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if(!result)
	{
		return false;
	}

	//��framebuffer�е�ͼ��present����Ļ��.

	m_pRender->EndScene();

	return true;
}
