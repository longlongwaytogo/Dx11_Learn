#pragma once
#pragma once
#include <windows.h>

class CDxInput;
class CDxGraphics;

class SystemApp
{
public:
	SystemApp(void);
	~SystemApp(void);


public:
	//��ʼ�������࣬����Ӧ�ó��򴰿� 
	void InitializeWindows(int& screenWidth, int& screenHeight); 
	//���ó�ʼ�����ں������Լ������Ľӿ���ĳ�ʼ������ 
	bool Initialize();


	void Frame();

	//������Ϣѭ�� 
	void Run();

	void Shutdown();
	//�رմ��� 
	void ShutdownWindows();

	//�����������Ǵ��ڵĻص����� 
	static LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM); 
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


private:
	CDxInput*	m_pInput;
	CDxGraphics* m_pGraphics;


public:
	static	LPCWSTR m_applicationName;		// Ӧ�ó���ʵ����� 
	static HINSTANCE m_hinstance;			// ����һ��ȫ�ֵ�windows���ھ����������������ʾӦ�ó���������ھ���� 
	static HWND m_hwnd;						// ���ھ�� 
	static bool m_bExit;					// �����ж��Ƿ�ESC��,����ESC�������˳����� 
	//static bool m_FULL_SCREEN;				// �ж��Ƿ�ȫ����ȫ��ģʽ�ʹ���ģʽ��ʹ�ò�ͬ�Ĵ������ڲ���


};


