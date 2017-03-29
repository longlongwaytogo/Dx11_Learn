#include "Camera.h"
#include <D3DX10math.h>
CCamera::CCamera()
{

	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;
}

CCamera::~CCamera()
{

}

void CCamera::SetPosition(float x, float y , float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
}

void CCamera::SetRotation(float x , float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
}

D3DXVECTOR3 CCamera::GetPosition()
{
	return D3DXVECTOR3(m_positionX, m_positionY, m_positionZ);
}


D3DXVECTOR3 CCamera::GetRotation()
{
	return D3DXVECTOR3(m_rotationX, m_rotationY, m_rotationZ);
}

D3DMATRIX CCamera::GetViewMatrix()
{
	return m_viewMatrix;
}

void CCamera::Render()
{
	D3DXVECTOR3 up, position, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;


	// ����up����Ϊ(0,1,0).
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// �����������λ��.
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	// ���������lookat�ķ���.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// �õ����ȵ�λ��ŷ����ת yaw (Y axis), pitch (X axis), �Լ� roll (Z axis) .
	pitch = m_rotationX * 0.0174532925f;
	yaw   = m_rotationY * 0.0174532925f;
	roll  = m_rotationZ * 0.0174532925f;

	// �õ���ת����.
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// �任lookat��up������һ��view�ܹ���ԭ�㱻��ȷ��ת.
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// ƽ����ת��������λ��.
	lookAt = position + lookAt;

	// ����view����.
	D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt, &up);

	return;
}
