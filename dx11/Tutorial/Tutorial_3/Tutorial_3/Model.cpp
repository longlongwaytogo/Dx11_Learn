#include "Model.h"
#include <d3d11.h>
#include <D3DX10math.h>
CModel::CModel():m_vertexCount(3),m_indexCount(3)
{

}

CModel::~CModel()
{

}

typedef struct tag_VertexType
{
	D3DXVECTOR3 position;
	D3DXVECTOR4 color;

} VertexType;
bool CModel::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices; 
	unsigned long* indices; 
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc; 
	D3D11_SUBRESOURCE_DATA vertexData,indexData;

	HRESULT result;

	//首先，我们创建2个临时缓冲存放顶点和索引数据，以便后面使用。.

	// 设置顶点缓冲大小为3，一个三角形. 
	m_vertexCount = 3;

	// 设置索引缓冲大小. 
	m_indexCount = 3;

	// 创建顶点临时缓冲. 
	vertices = new VertexType[m_vertexCount]; 
	if(!vertices) 
	{ 
		return false; 
	}

	// 创建索引缓冲. 
	indices = new unsigned long[m_indexCount]; 
	if(!indices) 
	{ 
		return false; 
	} 
	//创建顺时针方向的三角形，左手规则 
	// 设置顶点数据. 
	vertices[0].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);  // 左下 
	vertices[0].color = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);

	vertices[1].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);  // 中上. 
	vertices[1].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[2].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);  // 底右 
	vertices[2].color = D3DXVECTOR4(0.0f, 1.0f, 1.0f, 1.0f);

	// 设置索引缓冲数据. 
	indices[0] = 0;  // Bottom left. 
	indices[1] = 1;  // Top middle. 
	indices[2] = 2;  // Bottom right.

	// 设置顶点缓冲描述 
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT; 
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount; 
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; 
	vertexBufferDesc.CPUAccessFlags = 0; 
	vertexBufferDesc.MiscFlags = 0; 
	vertexBufferDesc.StructureByteStride = 0;

	// 指向保存顶点数据的临时缓冲. 
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0; 
	vertexData.SysMemSlicePitch = 0;

	// 创建顶点缓冲. 
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer); 
	if(FAILED(result)) 
	{ 
		return false; 
	}

	// 设置索引缓冲描述. 
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT; 
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount; 
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER; 
	indexBufferDesc.CPUAccessFlags = 0; 
	indexBufferDesc.MiscFlags = 0; 
	indexBufferDesc.StructureByteStride = 0;

	// 指向存临时索引缓冲. 
	indexData.pSysMem = indices; 
	indexData.SysMemPitch = 0; 
	indexData.SysMemSlicePitch = 0;

	// 创建索引缓冲. 
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer); 
	if(FAILED(result)) 
	{ 
		return false; 
	}

	// 释放临时缓冲. 
	delete [] vertices; 
	vertices = 0;

	delete [] indices; 
	indices = 0;

	return true; 

}

void CModel::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride; 
	unsigned int offset;

	// 设置顶点缓冲跨度和偏移. 
	stride = sizeof(VertexType); 
	offset = 0;

	//在input assemberl阶段绑定顶点缓冲，以便能够被渲染
	deviceContext->IASetVertexBuffers(0,1,&m_vertexBuffer,&stride,&offset);

	//在input assemberl阶段绑定索引缓冲，以便能够被渲染 
	deviceContext->IASetIndexBuffer(m_indexBuffer,DXGI_FORMAT_R32_UINT,0);

	// 设置体元语义，渲染三角形列表. 
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	return; 
}

int CModel::GetIndexCount()
{
	return m_indexCount;
}

bool CModel::Initialize(ID3D11Device* device)
{
	bool result;

	// 初始化顶点缓冲和顶点索引缓冲.
	result = InitializeBuffers(device);
	if(!result)
	{
		return false;
	}

	return true;
}
