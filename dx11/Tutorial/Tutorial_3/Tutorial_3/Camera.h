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
	float m_positionX, m_positionY, m_positionZ;//摄像机在世界坐标系中的位置
	float m_rotationX, m_rotationY, m_rotationZ;//摄像机的欧拉旋转角度



};
#endif // __TUTORIAL_3__H__