#pragma once
class CDx11Window
{
public:
	CDx11Window(void);
	~CDx11Window(void);


public:
	//��ʼ�������࣬����Ӧ�ó��򴰿� 
	void InitializeWindows(int& screenWidth, int& screenHeight); 
	//���ó�ʼ�����ں������Լ������Ľӿ���ĳ�ʼ������ 
	bool Initialize();


	//������Ϣѭ�� 
	void Run();

	//�رմ��� 
	void ShutdownWindows();

	//�����������Ǵ��ڵĻص����� 
	static LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM); 
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


private:
	//�����ж��Ƿ�ESC��,����ESC�������˳����� 
	


public:
	static	LPCWSTR m_applicationName;		// Ӧ�ó���ʵ����� 
	static HINSTANCE m_hinstance;			// ����һ��ȫ�ֵ�windows���ھ����������������ʾӦ�ó���������ھ���� 
	static HWND m_hwnd;						// ���ھ�� 
	static bool m_bExit;					// �����ж��Ƿ�ESC��,����ESC�������˳����� 
	static bool FULL_SCREEN;				// �ж��Ƿ�ȫ����ȫ��ģʽ�ʹ���ģʽ��ʹ�ò�ͬ�Ĵ������ڲ���


};

