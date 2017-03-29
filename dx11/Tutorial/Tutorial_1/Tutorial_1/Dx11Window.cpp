#include "StdAfx.h"
#include "Dx11Window.h"



LPCWSTR CDx11Window::m_applicationName = L"Tutorial";		
HINSTANCE CDx11Window::m_hinstance = NULL;			
HWND CDx11Window::m_hwnd = NULL;			
bool CDx11Window::m_bExit = false;					
bool CDx11Window::FULL_SCREEN = false;

CDx11Window::CDx11Window(void)
{
}


CDx11Window::~CDx11Window(void)
{
}

void CDx11Window::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc; 
	DEVMODE dmScreenSettings; 
	int posX, posY;


	// �õ�Ӧ�ó���ʵ����� 
	m_hinstance = GetModuleHandle(NULL);

	// Ӧ�ó������� 
	m_applicationName = L"Engine";

	// ���ô��������. 
	wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

	// ָ���ص�������windows���Զ����ûص���������������Ϣ�¼� 
	wc.lpfnWndProc   = WndProc; //ָ���ص����� 
	wc.cbClsExtra    = 0; 
	wc.cbWndExtra    = 0; 
	wc.hInstance     = m_hinstance; 
	wc.hIcon         = LoadIcon(NULL, IDI_WINLOGO); 
	wc.hIconSm       = wc.hIcon; 
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW); 
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); //Ĭ�Ϻ�ɫ���ں�ɫ����
	wc.lpszMenuName  = NULL; 
	wc.lpszClassName = m_applicationName; 
	wc.cbSize        = sizeof(WNDCLASSEX);

	// ע�ᴰ���� 
	RegisterClassEx(&wc);

	// �õ�windows����ֱ��� 
	screenWidth  = GetSystemMetrics(SM_CXSCREEN); 
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// �����Ƿ�ȫ�����ò�ͬ�ķֱ���. 
	if(FULL_SCREEN) 
	{ 
		//ȫ��ģʽ�£����ô��ڴ�СΪwindows����ֱ���. 
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings)); 
		dmScreenSettings.dmSize       = sizeof(dmScreenSettings); 
		dmScreenSettings.dmPelsWidth  = (unsigned long)screenWidth; 
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight; 
		dmScreenSettings.dmBitsPerPel = 32;            
		dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// ��ʱ������ʾ�豸Ϊȫ��ģʽ��ע�⣺Ӧ�ó����˳�ʱ�򣬽��ָ�ϵͳĬ�����á� 
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// ���ô��ڵ����Ͻ�����λ��Ϊ(0,0). 
		posX = posY = 0; 
	} 
	else 
	{ 
		// ����ģʽ��800*600. 
		screenWidth  = 800; 
		screenHeight = 600;

		// �������Ͻ�����λ��,posX, posY

		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth)  / 2; 
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2; 
	}

	// ȫ���ʹ���ʹ�ò�ͬ�Ĳ���. 
	if( FULL_SCREEN) 
	{ 
		m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, 
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, 
			posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL); 
	} 
	else 
	{ 
		m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, 
			WS_OVERLAPPEDWINDOW, 
			posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL); 
	}

	// ��ʾ���ڲ�������Ϊ����. 
	ShowWindow(m_hwnd, SW_SHOW); 
	SetForegroundWindow(m_hwnd); 
	SetFocus(m_hwnd);

	// �������. 
	//ShowCursor(false);

	return; 


}

bool CDx11Window::Initialize()
{
	int screenWidth = 0, screenHeight = 0;

	// ��ʼ������ 
	InitializeWindows(screenWidth, screenHeight);

	return true; 
}

void CDx11Window::Run()
{
	MSG msg; 
	bool done, result = 1;


	// ��ʼ����Ϣ�ṹ. 
	ZeroMemory(&msg, sizeof(MSG));

	// ѭ��������Ϣ����

	done = false; 
	while(!done) 
	{ 
		// ����windows��Ϣ. 
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{ 
			TranslateMessage(&msg); 
			DispatchMessage(&msg); 
		}

		// ���յ�WM_QUIT��Ϣ���˳�����. 
		if(msg.message == WM_QUIT) 
		{ 
			done = true; 
		} 
		else 
		{ 
			result = m_bExit; //�������ESC,Ҳ�˳�����

			//���ǵ���Ⱦ�����������������Է������ 
			//.... 
			//..... 
			if(result) 
			{ 
				done = true; 
			} 
		}

	}

	return; 
}

void CDx11Window::ShutdownWindows()
{
	//��ʾ���. 
	//ShowCursor(true);

	// �ָ�Ĭ����ʾ����. 
	if(FULL_SCREEN) 
	{ 
		ChangeDisplaySettings(NULL, 0); 
	}

	//�ͷŴ��ھ��. 
	DestroyWindow(m_hwnd); 
	m_hwnd = NULL;

	// �ͷ�Ӧ�ó���ʵ��. 
	UnregisterClass(m_applicationName, m_hinstance); 
	m_hinstance = NULL;

	return; 

}

LRESULT CALLBACK CDx11Window::MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch(umessage) 
	{ 
		// ��ⰴ����Ϣ. 
	case WM_KEYDOWN: 
		if(wparam==VK_ESCAPE) 
			m_bExit = true; 
		return 0; 
		//�κ�������Ϣ���͵�windowsȱʡ����. 
	default: 
		{ 
			return DefWindowProc(hwnd, umessage, wparam, lparam); 
		} 
	} 
}

LRESULT CALLBACK CDx11Window::WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch(umessage) 
	{

		// ����������Ϣ. 
	case WM_DESTROY: 
		{ 
			PostQuitMessage(0); 
			return 0; 
		}

		// ���ڹر���Ϣ. 
	case WM_CLOSE: 
		{ 
			PostQuitMessage(0); 
			return 0; 
		}

		//MessageHandle���̴�������������Ϣ. 
	default: 
		{ 
			return MessageHandler(hwnd, umessage, wparam, lparam); 
		} 
	} 
}