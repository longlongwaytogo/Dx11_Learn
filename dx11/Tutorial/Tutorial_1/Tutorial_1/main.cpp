// Tutorial_1.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Tutorial.h"

#include "Dx11Window.h"

#define MAX_LOADSTRING 100



int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	
		CDx11Window window;
		window.Initialize(); 
		window.Run(); 
		window.ShutdownWindows(); 
		return 0; 
}




