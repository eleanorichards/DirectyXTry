#include "stdafx.h"
#include "System.h"
#include "AntTweakBar.h"

SystemClass::SystemClass()
{
	m_Input = 0;
	m_Graphics = 0;	

}

SystemClass::SystemClass(const SystemClass &)
{
}

SystemClass::~SystemClass()
{
}

bool SystemClass::Initialize()
{
	int screenWidth, screenHeight;
	bool result;


	// Initialize the width and height of the screen to zero before sending the variables into the function.
	screenWidth = 0;
	screenHeight = 0;

	// Initialize the windows api.
	InitializeWindows(screenWidth, screenHeight);

	// Create the input object.  This object will be used to handle reading the keyboard input from the user.
	m_Input = new Input;
	if (!m_Input)
	{
		return false;
	}
	// Initialize the input object.
	result = m_Input->Initialise(m_hinstance, m_hwnd, screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}

	// Create the graphics object.  This object will handle rendering all the graphics for this application.
	m_Graphics = new Graphics;
	if (!m_Graphics)
	{
		return false;
	}

	// Initialize the graphics object.
	result = m_Graphics->Initialise(screenWidth, screenHeight, m_hwnd);
	if (!result)
	{
		return false;
	}

	return true;
	
}

void SystemClass::Shutdown()
{
	//Release the graphihics
	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}
	//Release Input
	if (m_Input)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input = 0;
	}
	//Shutdown the window
	ShutdownWindows();

	return;
}

	//THE MAIN LOOP
	void SystemClass::Run()
	{
		MSG msg;
		bool done = false;
		bool result = false;


		// Initialize the message structure.
		ZeroMemory(&msg, sizeof(MSG));

		// Loop until there is a quit message from the window or the user.
		while (!done)
		{
			// Handle the windows messages.
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			// If windows signals to end the application then exit out.
			if (msg.message == WM_QUIT)
			{
				done = true;
			}
			//else
			//{
				// Otherwise do the frame processing.  If frame processing fails then exit.
			result = Frame();
			if (!result)
			{
				MessageBox(m_hwnd, L"Frame Processing Failed", L"Error", MB_OK);
				done = true;
			}
			//}

				// Check if the user pressed escape and wants to quit.
			if (m_Input->IsEscapePressed() == true)
			{
				done = true;
			}
		}

		return;
	}


LRESULT SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	//switch (umsg)
	//{
	//	

	//	// Check if a key has been pressed on the keyboard.
	//	case WM_KEYDOWN:
	//	{
	//		// If a key is pressed send it to the input object so it can record that state.
	//		m_Input->KeyDown((unsigned int)wparam);
	//		return 0;
	//	}

	//	// Check if a key has been released on the keyboard.
	//	case WM_KEYUP:
	//	{
	//		// If a key is released then send it to the input object so it can unset the state for that key.
	//		m_Input->KeyUp((unsigned int)wparam);
	//		return 0;
	//	}

	//	// Any other messages send to the default message handler as our application won't make use of them.
	//	default:
	//	{
	//		return DefWindowProc(hwnd, umsg, wparam, lparam);
	//	}
	//}
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

bool SystemClass::Frame()
{
	bool result;
	int mouseX, mouseY;
	// Do the input frame processing.
	result = m_Input->Frame();
	if (!result)
	{
		return false;
	}
	// Get the location of the mouse from the input object,
	m_Input->GetMouseLocation(mouseX, mouseY);

	// Do the frame processing for the graphics object.
	result = m_Graphics->Frame();
	if (!result)
	{
		return false;
	}

	// Finally render the graphics to the screen.
	/*result = m_Graphics->Render(0.0f);
	if (!result)
	{
		return false;
	}*/

	return true;
}

void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;


	// Get an external pointer to this object.	
	ApplicationHandle = this;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = L"System";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	//ShowCursor(false);

	return;
}

void SystemClass::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}

void SystemClass::takeInput()
{
	//if(m_Input->isKeyDown(VK_LEFT))
	//{
	//	//SETsTATE.LEFT
	//	m_Graphics->MoveCamera(-0.1f,0,0);
	//	return ;
	//}
	//if (m_Input->isKeyDown(VK_RIGHT))
	//{
	//	m_Graphics->MoveCamera(0.1f, 0, 0);
	//	return;
	//}
	//if (m_Input->isKeyDown(VK_UP))
	//{
	//	m_Graphics->MoveCamera(0, 0.1f, 0);
	//	return;
	//}
	//if (m_Input->isKeyDown(VK_DOWN))
	//{
	//	m_Graphics->MoveCamera(0, -0.1f, 0);
	//	return;
	//}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	if (TwEventWin(hwnd, umessage, wparam, lparam)) // send event message to AntTweakBar
		return 0; // event has been handled by AntTweakBar
					// else process the event message
					// ...
	switch (umessage)
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

		// All other messages pass to the message handler in the system class.
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}