#pragma once
#include <windows.h>

///////////// 
// GLOBALS // 
///////////// 

const bool FULL_SCREEN = false; //�Ƿ�ȫ�� 
const bool VSYNC_ENABLED = true; //�Ƿ�ֱͬ�� 
const float SCREEN_DEPTH = 1000.0f; //��ȣ�Զ�� 
const float SCREEN_NEAR = 0.1f; //��ȣ�����

class Dx11Render;
class CCamera;
class CColorShader;
class CModel;

class CDxGraphics
{
public:
	CDxGraphics(void);
	~CDxGraphics(void);
	bool Initialize(int w,int h,HWND hwnd);
	void Shutdown();
	bool Frame();

private:
	 bool Render(); 

private:
	Dx11Render* m_pRender;
	CColorShader*				m_pColorShader;
	CCamera*					m_pCamera;
	CModel*						m_pModel;
};

