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

	// 初始所有的键都是非按下状态. 
	for(i=0; i<256; i++) 
	{ 
		m_keys[i] = false; 
	}

	return true;
}

void CDxInput::KeyDown(unsigned int input)
{

	//键被按下. 
	m_keys[input] = true; 
	return; 
}

void CDxInput::KeyUp(unsigned int input)
{
	//释放按键. 
	m_keys[input] = false; 
	return; 
}

bool CDxInput::IsKeyDown(unsigned int key)
{
	// 判断键是否被按下？ 
	return m_keys[key]; 

}
