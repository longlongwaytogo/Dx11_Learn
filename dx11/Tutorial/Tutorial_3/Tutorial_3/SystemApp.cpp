#include "SystemApp.h"

#include "DxInput.h"
#include "DxGraphics.h"


LPCWSTR SystemApp::m_applicationName = L"Tutorial";		
HINSTANCE SystemApp::m_hinstance = NULL;			
HWND SystemApp::m_hwnd = NULL;			
bool SystemApp::m_bExit = false;					
//bool SystemApp::FULL_SCREEN = false;

SystemApp::SystemApp(void):m_pInput(NULL),m_pGraphics(NULL)
{
}


SystemApp::~SystemApp(void)
{
}

void SystemApp::InitializeWindows(int& screenWidth, int& screenHeight)
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

	// ָ���ص�������windows���Զ����ûص��������������Ϣ�¼� 
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

bool SystemApp::Initialize()
{
	int screenWidth = 0, screenHeight = 0;

	// ��ʼ������ 
	InitializeWindows(screenWidth, screenHeight);


	m_pInput = new CDxInput; 
	if(!m_pInput) 
	{ 
		return false; 
	}

	// ��ʼ���������. 
	m_pInput->Initialize();

	// ����ͼ�ζ������������ȾӦ�ó����е��������� 
	m_pGraphics = new CDxGraphics; 
	if(!m_pGraphics) 
	{ 
		return false; 
	} 
	// ��ʼ��ͼ�ζ��� 
	bool result = m_pGraphics->Initialize(screenWidth, screenHeight, m_hwnd); 
	if(!result) 
	{ 
		return false; 
	}



	return true; 
}

void SystemApp::Frame()
{
	if(m_pGraphics)
		m_pGraphics->Frame();

}

void SystemApp::Run()
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
			Frame();
			result = m_bExit; //�������ESC,Ҳ�˳�����

			//���ǵ���Ⱦ���������������Է������ 
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

void SystemApp::Shutdown()
{
	if(m_pGraphics) 
	{ 
		m_pGraphics->Shutdown(); 
		delete m_pGraphics; 
		m_pGraphics = 0; 
	}

	if(m_pInput) 
	{ 
		delete m_pInput; 
		m_pInput = 0; 
	} 

	ShutdownWindows();
}

void SystemApp::ShutdownWindows()
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

LRESULT CALLBACK SystemApp::MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
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

LRESULT CALLBACK SystemApp::WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
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
