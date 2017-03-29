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
	Initialize����������ʼ��D3D11�豸���������״̬��

	��ʼ��D3D�Ĵ�����Ҫ�������¼������裺

	1. ����һ��������(swapchain)��������,�ñ��������󻺳������(��С����ʽ���ȡ�

	2. ͨ������D3D11CreateDeviceAndSwapChain����D3D11 �豸���������豸(ע����D3D10�У�ֻ��һ��device)��


	3. ����һ����ȾĿ����ͼָ��󻺳塣

	4. ������ȡ�ģ�滺���Լ���Ӧ����ͼ��

	5. ������ȡ�ģ��״̬����OM(����ϲ�)�׶ΰ󶨵����ߡ�

	6. ������������Ⱦ״̬��

	7. �����ӿ�
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

	// ����һ��DirectX graphics interface factory.  
	result = CreateDXGIFactory(__uuidof(IDXGIFactory),(void**)&factory);
	if(FAILED(result))
	{
		return false;
	}

	 // �ýӿڹ�������һ�����Կ������� 
	result = factory->EnumAdapters(0,&adapter);
	if(FAILED(result))
	{
		return false;
	}

	//  // �õ��������������. 
	result = adapter->EnumOutputs(0,&adapterOutPut);
	if(FAILED(result))
	{
		return false;
	}
	    //�õ��ʺ� DXGI_FORMAT_R8G8B8A8_UNORM ����ʾģʽ. 
	result = adapterOutPut->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,DXGI_ENUM_MODES_INTERLACED,&numModes,NULL);

	if(FAILED(result))
	{

		return false;
	}
	displayModeList = new DXGI_MODE_DESC[numModes];
	if(!displayModeList)
		return false;

	//   // ������ʾģʽ��displayModeList�� 
	result = adapterOutPut->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,DXGI_ENUM_MODES_INTERLACED,&numModes,displayModeList);
	if(FAILED(result))
	{
		return false;
	}

	//����������ʾģʽ���õ�ˢ������������ֵnumerator �� denominator 
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

	// �õ��Կ����� 
	result = adapter->GetDesc(&adapterDesc); 
	if(FAILED(result)) 
	{ 
		return false; 
	}

	// �����Դ��С. 
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	//�����Կ�������

	//wcstombs_s, wide charת��Ϊchar 
	error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128); 
	if(error != 0) 
	{ 
		return false; 
	} 
	// �ͷ���ʾģʽ�б� 
	delete [] displayModeList; 
	displayModeList = 0;

	//�ͷ����������. 
	adapterOutPut->Release(); 
	adapterOutPut = 0;

	//�ͷ������� 
	adapter->Release(); 
	adapter = 0;

	// ��ʼ������������ 
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// ��1���󻺳� 
	swapChainDesc.BufferCount = 1;

	//֡����Ĵ�С��Ӧ�ó��򴰿ڴ�С���  
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	// �󻺳��surface�ĸ�ʽΪDXGI_FORMAT_R8G8B8A8_UNORM   
	// surface��ÿ��������4���޷��ŵ�8bit[ӳ�䵽0-1]����ʾ��NORM��ʾ��һ��  
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// ���ʹ�ô�ֱͬ�������ú󻺳��ˢ����

	//ˢ���ʾ���һ���ӰѺ󻺳���������Ļ�ϻ����Ĵ���  
	//���������ֱͬ����������ˢ���ʣ���fps�ǹ̶���


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

	// ���ú󻺳����; 
	// ���ǵ���ȾĿ�껺��Ϊ�󻺳�  
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// �󻺳�����Ĵ��ھ��  
	swapChainDesc.OutputWindow = hwnd;
	// ��ʹ�ö��ز��� 
	swapChainDesc.SampleDesc.Count = 1; 
	swapChainDesc.SampleDesc.Quality = 0;


	// ����ȫ�����ߴ���ģʽ  
	if(fullscreen) 
	{ 
		swapChainDesc.Windowed = false; 
	} 
	else 
	{ 
		swapChainDesc.Windowed = true; 
	}

	// �趨ɨ����ordering�Լ�����Ϊunspecified  
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; 
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// �󻺳����ݳ��ֵ���Ļ�󣬷��������� 
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	//�����ñ�־ 
	swapChainDesc.Flags = 0;

	// ����feature levelΪD3D11 
	// ����Կ���֧��D3D11,�����ܹ�ͨ���������������ʹ��D3D10,����9  
	featureLevel = D3D_FEATURE_LEVEL_11_0;

#if 0
	// �������������豸�Լ��豸������  
	result = D3D11CreateDeviceAndSwapChain(adapter,D3D_DRIVER_TYPE_HARDWARE,NULL,0,&featureLevel,1,
		D3D11_SDK_VERSION,&swapChainDesc,&m_pSwapChain,&m_pD3dDevice,NULL,&m_pD3dDeviceContext);

	if(FAILED(result)) 
	{ 
		return false; 
	}
#else
	result = D3D11CreateDevice(NULL, //Ĭ���Կ�
		D3D_DRIVER_TYPE_HARDWARE,
		0,//��ʾû�����device
		0,//�����豸�ı�־������ָ��ΪD3D11_CREATE_DEVICE_DEBUG������vs�����������Ϣ
		0,0,//FeatureLevel����Ϣ������Ϊ0����ʹ����ߵ�feature��Ҳ����D3D11 feature
		D3D11_SDK_VERSION, //SDK�汾
		&m_pD3dDevice,
		&featureLevel,//��������ѡ���feature level��Ϣ
		&m_pD3dDeviceContext
		);

	if(FAILED(result))
	{

		return false;
	}

	UINT m4xMsaaQuality;

	m_pD3dDevice->CheckMultisampleQualityLevels( DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality);

	//��������������0�����ʾ��ʾ��ʽ�Ͳ������������Ӳ����֧�֡����򣬷�����Ч��quality��Ŀ����Ч��qualityֵ��0-m4xMsaaQuality��

	//	֮����ԣ���������swapchain��

	result = factory->CreateSwapChain(m_pD3dDevice,&swapChainDesc, &m_pSwapChain);

	if(FAILED(result))
	{

		return false;
	}


#endif 


	// �ͷŽӿڹ���. 
	factory->Release(); 
	factory = 0;

	// �õ��������еĺ󻺳�ָ��  
	result = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr); 
	if(FAILED(result)) 
	{ 
		return false; 
	}
	 // �ú󻺳崴����ȾĿ����ͼ. 
	 result = m_pD3dDevice->CreateRenderTargetView(backBufferPtr,NULL,&m_pRenderTargetView);
	 if(FAILED(result))
	 {
		 return false;
	 }

	 //�ͷź󻺳�(���ü�����1) 
	 backBufferPtr->Release(); 
	 backBufferPtr = 0;

	 // ��ʼ����Ȼ�������. 
	 ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	 //������Ȼ������� 
	 depthBufferDesc.Width = screenWidth; 
	 depthBufferDesc.Height = screenHeight; 
	 depthBufferDesc.MipLevels = 1; //������Ȼ���Ϊ1 
	 depthBufferDesc.ArraySize = 1; //������Ȼ���Ϊ1������������2�������и�����; 
	 depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; 
	 depthBufferDesc.SampleDesc.Count = 1; 
	 depthBufferDesc.SampleDesc.Quality = 0; 
	 depthBufferDesc.Usage = D3D11_USAGE_DEFAULT; 
	 depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL; 
	 depthBufferDesc.CPUAccessFlags = 0; 
	 depthBufferDesc.MiscFlags = 0;

	 // ������Ȼ��� 
	 result = m_pD3dDevice->CreateTexture2D(&depthBufferDesc,NULL,&m_pDepthBackBuf);
	 if(FAILED(result)) 
	 { 
		 return false;

	 }

	   // ��ʼ�����ģ��״̬���� 
	 ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	 // �������ģ��״̬���� 
	 depthStencilDesc.DepthEnable = true; 
	 depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

	 //D3D11_DEPTH_WRITE_MASK_ZERO��ֹд��Ȼ��� 
	 depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	 depthStencilDesc.StencilEnable = true; 
	 depthStencilDesc.StencilReadMask = 0xFF; 
	 depthStencilDesc.StencilWriteMask = 0xFF;

	 // ����front face ����ʹ�õ�ģ���������

	 depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP; 
	 depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR; 
	 depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP; 
	 depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	 // ����back face����ʹ�õ�ģ�����ģʽ 
	 depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP; 
	 depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR; 
	 depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP; 
	 depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	 //�������ģ��״̬ 
	 result = m_pD3dDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState); 
	 if(FAILED(result)) 
	 { 
		 return false;

	 }

	 // �������ģ��״̬��ʹ����Ч 
	 m_pD3dDeviceContext->OMSetDepthStencilState(m_pDepthStencilState,1);


	 // ��ʼ�����ģ����ͼ. 
	 ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	 // �������ģ����ͼ����. 
	 depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; 
	 depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D; 
	 depthStencilViewDesc.Texture2D.MipSlice = 0;

	 // �������ģ����ͼ. 
	 result = m_pD3dDevice->CreateDepthStencilView(m_pDepthBackBuf, &depthStencilViewDesc, &m_pDepthStencilView); 
	 if(FAILED(result)) 
	 { 
		 return false; 
	 }

	 //  ����ȾĿ����ͼ����Ȼ��嵽��Ⱦ����. 
	 m_pD3dDeviceContext->OMSetRenderTargets(1,&m_pRenderTargetView,m_pDepthStencilView);

	 // ���ù�դ��������ָ���������α���Ⱦ. 
	 rasterDesc.AntialiasedLineEnable = false; 
	 rasterDesc.CullMode = D3D11_CULL_BACK; //�����޳��������������λᱻ�޳����� 
	 rasterDesc.DepthBias = 0; 
	 rasterDesc.DepthBiasClamp = 0.0f; 
	 rasterDesc.DepthClipEnable = true; 
	 rasterDesc.FillMode = D3D11_FILL_SOLID; 
	 rasterDesc.FrontCounterClockwise = false; 
	 rasterDesc.MultisampleEnable = false; 
	 rasterDesc.ScissorEnable = false; 
	 rasterDesc.SlopeScaledDepthBias = 0.0f;

	 // ������դ��״̬. 
	 result = m_pD3dDevice->CreateRasterizerState(&rasterDesc, &m_pRasterState); 
	 if(FAILED(result)) 
	 { 
		 return false; 
	 }

	 //���ù�դ��״̬��ʹ����Ч 
	 m_pD3dDeviceContext->RSSetState(m_pRasterState);


	 // �����ӿڣ���ʾȫ���󻺳����� 
	 viewport.Width = (float)screenWidth; 
	 viewport.Height = (float)screenHeight; 
	 viewport.MinDepth = 0.0f; 
	 viewport.MaxDepth = 1.0f; 
	 viewport.TopLeftX = 0.0f; 
	 viewport.TopLeftY = 0.0f;

	 m_pD3dDeviceContext->RSSetViewports(1,&viewport);


	 // ����͸��ͶӰ���� 
	 fieldOfView = (float)D3DX_PI/ 4.0f;
	 screenAspect = (float)screenWidth / (float)screenHeight;

	 //����͸��ͶӰ����
	 
	 D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, fieldOfView,screenAspect,screenNear,screenDepth);
	 //��ʼ��world����Ϊ��λ����.

	 //�þ���ʵ�־ֲ����굽���������ת�� 
	 D3DXMatrixIdentity(&m_worldMatrix);


	 // ��������ͶӰ������Ҫ����ʵʩ2D��Ⱦ. 
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
	// ���������֡����(��������Ⱦ������
	m_pD3dDeviceContext->ClearRenderTargetView(m_pRenderTargetView,color);
	// ����������ģ�滺�壬Ȼ��ʼ��Ⱦ3D����
	m_pD3dDeviceContext->ClearDepthStencilView(m_pDepthStencilView,D3D11_CLEAR_DEPTH,1.0f,0);
}



void Dx11Render::EndScene()
{

	//��Ⱦ��ɺ����ǻ�Ҫ��Present�����Ѻ󻺳�������ʾ������豸�ϡ�
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