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
	//初始化窗口类，创建应用程序窗口 
	void InitializeWindows(int& screenWidth, int& screenHeight); 
	//调用初始化窗口函数，以及其它的接口类的初始化函数 
	bool Initialize();


	void Frame();

	//处理消息循环 
	void Run();

	void Shutdown();
	//关闭窗口 
	void ShutdownWindows();

	//这两个函数是窗口的回调函数 
	static LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM); 
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


private:
	CDxInput*	m_pInput;
	CDxGraphics* m_pGraphics;


public:
	static	LPCWSTR m_applicationName;		// 应用程序实例句柄 
	static HINSTANCE m_hinstance;			// 定义一个全局的windows窗口句柄，这个句柄用来表示应用程序的主窗口句柄。 
	static HWND m_hwnd;						// 窗口句柄 
	static bool m_bExit;					// 用来判断是否按ESC键,按下ESC键，则退出程序 
	//static bool m_FULL_SCREEN;				// 判断是否全屏，全屏模式和窗口模式下使用不同的创建窗口参数


};


