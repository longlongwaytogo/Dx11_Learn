// Tutorial_1.cpp : 定义应用程序的入口点。
//




#include "SystemApp.h"
#include <tchar.h>
#define MAX_LOADSTRING 100



int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	
		SystemApp app;
		if(!app.Initialize()) return 0; 
		app.Run(); 
		app.Shutdown(); 
		return 0; 
}




