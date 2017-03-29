#include "DxInput.h"


CDxInput::CDxInput(void)
{
}


CDxInput::~CDxInput(void)
{
}

bool CDxInput::Initialize()
{
	int i;

	// ��ʼ���еļ����Ƿǰ���״̬. 
	for(i=0; i<256; i++) 
	{ 
		m_keys[i] = false; 
	}

	return true;
}

void CDxInput::KeyDown(unsigned int input)
{

	//��������. 
	m_keys[input] = true; 
	return; 
}

void CDxInput::KeyUp(unsigned int input)
{
	//�ͷŰ���. 
	m_keys[input] = false; 
	return; 
}

bool CDxInput::IsKeyDown(unsigned int key)
{
	// �жϼ��Ƿ񱻰��£� 
	return m_keys[key]; 

}
