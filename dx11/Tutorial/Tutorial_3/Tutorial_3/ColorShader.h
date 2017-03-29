#pragma once
#include <d3d11.h>
#include <D3DX10math.h>

class CColorShader
{
public:
	CColorShader();
	~CColorShader();


	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();

	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, 
		D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix) ;
private:
	bool  InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename);
	void ShutdownShader();
	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, 
		D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename);

	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

private:
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader*  m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;

};

///////////// 
// GLOBALS // 
//shader中使用的全局变量都在定义在const buffer中 
//这样shader编译后，这些变量放在gpu的const buffer中 
///////////// 
