#pragma once
// Filename: systemclass.h
#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

// PRE-PROCESSING DIRECTIVES //
#define WIN32_LEAN_AND_MEAN


// INCLUDES //
#include <windows.h>


// MY CLASS INCLUDES //
#include "Input.h"
#include "Graphics.h"

// Class name: SystemClass
class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();
	void takeInput();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	Input* m_Input;
	Graphics* m_Graphics;
};


/////////////////////////
// FUNCTION PROTOTYPES //
/////////////////////////
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


/////////////
// GLOBALS //
/////////////
static SystemClass* ApplicationHandle = 0;


#endif