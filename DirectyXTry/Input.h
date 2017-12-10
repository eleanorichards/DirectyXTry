#ifndef _INPUT_H_
#define _INPUT_H_
// PRE-PROCESSING DIRECTIVES //
#define DIRECTINPUT_VERSION 0x0800
//LINKING
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
//includes
#include <dinput.h>

class  Input
{
public:
	 Input();
	 Input(const Input&);
	~ Input();

	bool Initialise(HINSTANCE, HWND, int, int);

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);
	bool isKeyDown(unsigned int);

	bool IsLeftMouseButtonDown();
	void Shutdown();
	bool Frame();

	bool IsEscapePressed();
	void GetMouseLocation(float&, float&);
private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();
private:
	//bool m_keys[256];	
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;
	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;
};
#endif // !_INPUT_H_
