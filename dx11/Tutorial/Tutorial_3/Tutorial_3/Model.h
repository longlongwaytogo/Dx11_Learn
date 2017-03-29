#pragma once
#include <d3d11.h>
class CModel
{
public:
	CModel();
	~CModel();

	bool InitializeBuffers(ID3D11Device* device);
	void RenderBuffers(ID3D11DeviceContext* deviceContext);
	int GetIndexCount();
	bool Initialize(ID3D11Device* device);

private:
	int m_vertexCount;
	int m_indexCount;
	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;

};