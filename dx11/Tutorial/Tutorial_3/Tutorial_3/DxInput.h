#pragma once
class CDxInput
{
public:
	CDxInput(void);
	~CDxInput(void);

	bool Initialize();
	void KeyDown(unsigned int); 
	void KeyUp(unsigned int);
	bool IsKeyDown(unsigned int);

private: 
	bool m_keys[256];//�������256��������Ƿ񱻰��� 
};

