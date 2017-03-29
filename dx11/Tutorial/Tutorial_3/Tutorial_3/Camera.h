#ifndef __TUTORIAL_3__H__
#define __TUTORIAL_3__H__
#include <D3DX10math.h>

class CCamera
{


public:
	CCamera();
	~CCamera();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();


	D3DMATRIX GetViewMatrix();

	void Render();

private:

	D3DXMATRIX m_viewMatrix;
	float m_positionX, m_positionY, m_positionZ;//���������������ϵ�е�λ��
	float m_rotationX, m_rotationY, m_rotationZ;//�������ŷ����ת�Ƕ�



};
#endif // __TUTORIAL_3__H__