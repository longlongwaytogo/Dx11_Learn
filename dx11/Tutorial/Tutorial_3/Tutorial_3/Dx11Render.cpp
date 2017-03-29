#include "Dx11Render.h"


Dx11Render::Dx11Render(void):m_vsync_enabled(false),m_pSwapChain(NULL),m_pD3dDevice(NULL),
	m_pD3dDeviceContext(NULL),m_pDepthStencilState(NULL),	m_pDepthStencilView(NULL),
	m_pRenderTargetView(NULL),m_pDepthBackBuf(NULL),m_pRasterState(NULL)
{
}


Dx11Render::~Dx11Render(void)
{
}

bool Dx11Render::Initialize(int screenWidth, int screenHeight, HWND hwnd,bool vsync,bool fullscreen, 
	float screenDepth, float screenNear)
{
	/*
	Initialize函数，来初始化D3D11设备，设置相关状态。

	初始化D3D的代码主要包括以下几个步骤：

	1. 创建一个交换链(swapchain)描述变量,该变量包括后缓冲的设置(大小，格式）等。

	2. 通过函数D3D11CreateDeviceAndSwapChain创建D3D11 设备和上下文设备(注：在D3D10中，只有一个device)。


	3. 创建一个渲染目标视图指向后缓冲。

	4. 创建深度、模版缓冲以及相应的视图。

	5. 创建深度、模版状态并在OM(输出合并)阶段绑定到管线。

	6. 创建并设置渲染状态。

	7. 设置视口
	*/

	HRESULT result;
	IDXGIFactory* factory = NULL;
	IDXGIAdapter* adapter = NULL;
	IDXGIOutput* adapterOutPut = NULL;
	unsigned int numModes,i,numerator,denominator,stringLength;
	DXGI_MODE_DESC* displayModeList = NULL;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr = NULL;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc; 
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc; 
	D3D11_RASTERIZER_DESC rasterDesc; 
	D3D11_VIEWPORT viewport; 
	float fieldOfView, screenAspect;

	// ch
	m_vsync_enabled = vsync;

	// 创建一个DirectX graphics interface factory.  
	result = CreateDXGIFactory(__uuidof(IDXGIFactory),(void**)&factory);
	if(FAILED(result))
	{
		return false;
	}

	 // 用接口工厂创建一个主显卡的适配 
	result = factory->EnumAdapters(0,&adapter);
	if(FAILED(result))
	{
		return false;
	}

	//  // 得到主适配器的输出. 
	result = adapter->EnumOutputs(0,&adapterOutPut);
	if(FAILED(result))
	{
		return false;
	}
	    //得到适合 DXGI_FORMAT_R8G8B8A8_UNORM 的显示模式. 
	result = adapterOutPut->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,DXGI_ENUM_MODES_INTERLACED,&numModes,NULL);

	if(FAILED(result))
	{

		return false;
	}
	displayModeList = new DXGI_MODE_DESC[numModes];
	if(!displayModeList)
		return false;

	//   // 保存显示模式到displayModeList中 
	result = adapterOutPut->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,DXGI_ENUM_MODES_INTERLACED,&numModes,displayModeList);
	if(FAILED(result))
	{
		return false;
	}

	//遍历所有显示模式，得到刷新率两个参数值numerator 和 denominator 
	for(i=0; i<numModes; i++) 
	{ 
		if(displayModeList[i].Width == (unsigned int)screenWidth) 
		{ 
			if(displayModeList[i].Height == (unsigned int)screenHeight) 
			{ 
				numerator = displayModeList[i].RefreshRate.Numerator; 
				denominator = displayModeList[i].RefreshRate.Denominator; 
			} 
		} 
	} 

	// 得到显卡描述 
	result = adapter->GetDesc(&adapterDesc); 
	if(FAILED(result)) 
	{ 
		return false; 
	}

	// 保存显存大小. 
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	//保存显卡描述串

	//wcstombs_s, wide char转化为char 
	error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128); 
	if(error != 0) 
	{ 
		return false; 
	} 
	// 释放显示模式列表 
	delete [] displayModeList; 
	displayModeList = 0;

	//释放适配器输出. 
	adapterOutPut->Release(); 
	adapterOutPut = 0;

	//释放适配器 
	adapter->Release(); 
	adapter = 0;

	// 初始化交换链描述 
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// 用1个后缓冲 
	swapChainDesc.BufferCount = 1;

	//帧缓冲的大小和应用程序窗口大小相等  
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	// 后缓冲的surface的格式为DXGI_FORMAT_R8G8B8A8_UNORM   
	// surface的每个像素用4个无符号的8bit[映射到0-1]来表示。NORM表示归一化  
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// 如果使用垂直同步，设置后缓冲的刷新率

	//刷新率就是一秒钟把后缓冲内容在屏幕上画出的次数  
	//如果开启垂直同步，则锁定刷新率，则fps是固定的


	if(m_vsync_enabled) 
	{ 
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator; 
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator; 
	} 
	else 
	{ 
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0; 
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1; 
	}

	// 设置后缓冲的用途 
	// 我们的渲染目标缓冲为后缓冲  
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// 后缓冲输出的窗口句柄  
	swapChainDesc.OutputWindow = hwnd;
	// 不使用多重采样 
	swapChainDesc.SampleDesc.Count = 1; 
	swapChainDesc.SampleDesc.Quality = 0;


	// 设置全屏或者窗口模式  
	if(fullscreen) 
	{ 
		swapChainDesc.Windowed = false; 
	} 
	else 
	{ 
		swapChainDesc.Windowed = true; 
	}

	// 设定扫描线ordering以及缩放为unspecified  
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; 
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// 后缓冲内容呈现到屏幕后，放弃其内容 
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	//不设置标志 
	swapChainDesc.Flags = 0;

	// 设置feature level为D3D11 
	// 如果显卡不支持D3D11,我们能够通过设置这个参数，使用D3D10,或者9  
	featureLevel = D3D_FEATURE_LEVEL_11_0;

#if 0
	// 创建交换链，设备以及设备上下文  
	result = D3D11CreateDeviceAndSwapChain(adapter,D3D_DRIVER_TYPE_HARDWARE,NULL,0,&featureLevel,1,
		D3D11_SDK_VERSION,&swapChainDesc,&m_pSwapChain,&m_pD3dDevice,NULL,&m_pD3dDeviceContext);

	if(FAILED(result)) 
	{ 
		return false; 
	}
#else
	result = D3D11CreateDevice(NULL, //默认显卡
		D3D_DRIVER_TYPE_HARDWARE,
		0,//表示没有软件device
		0,//创建设备的标志，可以指定为D3D11_CREATE_DEVICE_DEBUG，可在vs中输入调试信息
		0,0,//FeatureLevel的信息，设置为0，将使用最高的feature，也就是D3D11 feature
		D3D11_SDK_VERSION, //SDK版本
		&m_pD3dDevice,
		&featureLevel,//返回现在选择的feature level信息
		&m_pD3dDeviceContext
		);

	if(FAILED(result))
	{

		return false;
	}

	UINT m4xMsaaQuality;

	m_pD3dDevice->CheckMultisampleQualityLevels( DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality);

	//如果这个函数返回0，则表示显示格式和采样数量的组合硬件不支持。否则，返回有效的quality数目，有效的quality值是0-m4xMsaaQuality。

	//	之后可以，单独创建swapchain：

	result = factory->CreateSwapChain(m_pD3dDevice,&swapChainDesc, &m_pSwapChain);

	if(FAILED(result))
	{

		return false;
	}


#endif 


	// 释放接口工厂. 
	factory->Release(); 
	factory = 0;

	// 得到交换链中的后缓冲指针  
	result = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr); 
	if(FAILED(result)) 
	{ 
		return false; 
	}
	 // 用后缓冲创建渲染目标视图. 
	 result = m_pD3dDevice->CreateRenderTargetView(backBufferPtr,NULL,&m_pRenderTargetView);
	 if(FAILED(result))
	 {
		 return false;
	 }

	 //释放后缓冲(引用计数减1) 
	 backBufferPtr->Release(); 
	 backBufferPtr = 0;

	 // 初始化深度缓冲描述. 
	 ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	 //设置深度缓冲描述 
	 depthBufferDesc.Width = screenWidth; 
	 depthBufferDesc.Height = screenHeight; 
	 depthBufferDesc.MipLevels = 1; //对于深度缓冲为1 
	 depthBufferDesc.ArraySize = 1; //对于深度缓冲为1，对于纹理，这2个参数有更多用途 
	 depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; 
	 depthBufferDesc.SampleDesc.Count = 1; 
	 depthBufferDesc.SampleDesc.Quality = 0; 
	 depthBufferDesc.Usage = D3D11_USAGE_DEFAULT; 
	 depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL; 
	 depthBufferDesc.CPUAccessFlags = 0; 
	 depthBufferDesc.MiscFlags = 0;

	 // 创建深度缓冲 
	 result = m_pD3dDevice->CreateTexture2D(&depthBufferDesc,NULL,&m_pDepthBackBuf);
	 if(FAILED(result)) 
	 { 
		 return false;

	 }

	   // 初始化深度模版状态描述 
	 ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	 // 设置深度模版状态描述 
	 depthStencilDesc.DepthEnable = true; 
	 depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

	 //D3D11_DEPTH_WRITE_MASK_ZERO禁止写深度缓冲 
	 depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	 depthStencilDesc.StencilEnable = true; 
	 depthStencilDesc.StencilReadMask = 0xFF; 
	 depthStencilDesc.StencilWriteMask = 0xFF;

	 // 对于front face 像素使用的模版操作操作

	 depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP; 
	 depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR; 
	 depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP; 
	 depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	 // 对于back face像素使用的模版操作模式 
	 depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP; 
	 depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR; 
	 depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP; 
	 depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	 //创建深度模版状态 
	 result = m_pD3dDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState); 
	 if(FAILED(result)) 
	 { 
		 return false;

	 }

	 // 设置深度模版状态，使其生效 
	 m_pD3dDeviceContext->OMSetDepthStencilState(m_pDepthStencilState,1);


	 // 初始化深度模版视图. 
	 ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	 // 设置深度模版视图描述. 
	 depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; 
	 depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D; 
	 depthStencilViewDesc.Texture2D.MipSlice = 0;

	 // 创建深度模版视图. 
	 result = m_pD3dDevice->CreateDepthStencilView(m_pDepthBackBuf, &depthStencilViewDesc, &m_pDepthStencilView); 
	 if(FAILED(result)) 
	 { 
		 return false; 
	 }

	 //  绑定渲染目标视图和深度缓冲到渲染管线. 
	 m_pD3dDeviceContext->OMSetRenderTargets(1,&m_pRenderTargetView,m_pDepthStencilView);

	 // 设置光栅化描述，指定多边形如何被渲染. 
	 rasterDesc.AntialiasedLineEnable = false; 
	 rasterDesc.CullMode = D3D11_CULL_BACK; //背面剔除，看不见三角形会被剔除掉。 
	 rasterDesc.DepthBias = 0; 
	 rasterDesc.DepthBiasClamp = 0.0f; 
	 rasterDesc.DepthClipEnable = true; 
	 rasterDesc.FillMode = D3D11_FILL_SOLID; 
	 rasterDesc.FrontCounterClockwise = false; 
	 rasterDesc.MultisampleEnable = false; 
	 rasterDesc.ScissorEnable = false; 
	 rasterDesc.SlopeScaledDepthBias = 0.0f;

	 // 创建光栅化状态. 
	 result = m_pD3dDevice->CreateRasterizerState(&rasterDesc, &m_pRasterState); 
	 if(FAILED(result)) 
	 { 
		 return false; 
	 }

	 //设置光栅化状态，使其生效 
	 m_pD3dDeviceContext->RSSetState(m_pRasterState);


	 // 设置视口，显示全部后缓冲内容 
	 viewport.Width = (float)screenWidth; 
	 viewport.Height = (float)screenHeight; 
	 viewport.MinDepth = 0.0f; 
	 viewport.MaxDepth = 1.0f; 
	 viewport.TopLeftX = 0.0f; 
	 viewport.TopLeftY = 0.0f;

	 m_pD3dDeviceContext->RSSetViewports(1,&viewport);


	 // 设置透视投影矩阵 
	 fieldOfView = (float)D3DX_PI/ 4.0f;
	 screenAspect = (float)screenWidth / (float)screenHeight;

	 //创建透视投影矩阵
	 
	 D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, fieldOfView,screenAspect,screenNear,screenDepth);
	 //初始化world矩阵为单位矩阵.

	 //该矩阵实现局部坐标到世界坐标的转换 
	 D3DXMatrixIdentity(&m_worldMatrix);


	 // 创建正交投影矩阵，主要用来实施2D渲染. 
	 D3DXMatrixOrthoLH(&m_orthoMatrix, (float)screenWidth, (float)screenHeight, screenNear, screenDepth);




	return true;
}

void Dx11Render::Frame()
{


}



void Dx11Render::BeginScene(float r, float g, float b, float a)
{
	float color[] = { r,g,b,a};

	// render
	// 函数来清除帧缓冲(即设置渲染背景）
	m_pD3dDeviceContext->ClearRenderTargetView(m_pRenderTargetView,color);
	// 函数清除深度模版缓冲，然后开始渲染3D场景
	m_pD3dDeviceContext->ClearDepthStencilView(m_pDepthStencilView,D3D11_CLEAR_DEPTH,1.0f,0);
}



void Dx11Render::EndScene()
{

	//渲染完成后，我们还要用Present函数把后缓冲内容显示到输出设备上。
	m_pSwapChain->Present(0,0);
}

void Dx11Render::Shutdown()
{

}

ID3D11Device* Dx11Render::GetDevice()
{
	return m_pD3dDevice;
}

ID3D11DeviceContext* Dx11Render::GetDeviceContext()
{
	return m_pD3dDeviceContext;
}

IDXGISwapChain* Dx11Render::GetSwapChain()
{
	return m_pSwapChain;

}


void Dx11Render::GetProjectionMatrix(D3DXMATRIX& projectionMatrix)
{
	projectionMatrix = m_projectionMatrix;
	return;
}


void Dx11Render::GetWorldMatrix(D3DXMATRIX& worldMatrix)
{
	worldMatrix = m_worldMatrix;
	return;
}


void Dx11Render::GetOrthoMatrix(D3DXMATRIX& orthoMatrix)
{
	orthoMatrix = m_orthoMatrix;
	return;
}