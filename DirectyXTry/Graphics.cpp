#include "stdafx.h"
#include "Graphics.h"
#include "AntTweakBar.h"

Graphics::Graphics()
{
	m_Direct3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_TextureShader = 0;
	m_LightShader = 0;
	m_Light = 0;
	//COLOURSHADER
	//m_ColourShader = 0;	

}

Graphics::Graphics(const Graphics &)
{
}

Graphics::~Graphics()
{
}

bool Graphics::Initialise(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	// Create the Direct3D object.
	m_Direct3D = new D3DClass;
	if (!m_Direct3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialise Direct3D", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new Camera;
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);

	// Create the model object.
	m_Model = new Models;
	if (!m_Model)
	{
		return false;
	}

	// Initialize the model object.
	result = m_Model->Initialise(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(),"../DirectyXTry/Assets/Cube.txt", "../DirectyXTry/Assets/Stone03.tga");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	//COLOURSHADER
	/*Create the color shader object.
	m_ColourShader = new ColourShader;
	if (!m_ColourShader)
	{
		return false;
	}

	//// Initialize the color shader object.
	//result = m_ColourShader->Initialise(m_Direct3D->GetDevice(), hwnd);
	//if (!result)
	//{
	//	MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
	//	return false;
	//}*/

	// Create the texture shader object.
	m_TextureShader = new TextureShader;
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialise(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialise the color shader object.", L"Error", MB_OK);
		return false;
	}

	//AntTweakBar Initialisation
	TwInit(TW_DIRECT3D11, m_Direct3D->GetDevice());
	TwWindowSize(screenWidth, screenWidth);

	//create antTweakbars
	TwBar* bTweakBar;
	bTweakBar = TwNewBar("Model Info");
	TwAddVarRW(bTweakBar, "Rotation Speed", TW_TYPE_FLOAT, &rotation_speed, "min=0 max=10 step=0.1");
	TwAddVarRW(bTweakBar, "Rotation Speed2", TW_TYPE_FLOAT, &rotation_speed, "keyIncr=i keyDecr=p");
	

	//LIGHT STUFF

	// Create the light shader object.
	//m_LightShader = new LightShader;
	//if (!m_LightShader)
	//{
	//	return false;
	//}

	//// Initialize the light shader object.
	////result = m_LightShader->Initialise(m_Direct3D->GetDevice(), hwnd);
	//if (!result)
	//{
	//	MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
	//	return false;
	//}

	//// Create the light object.
	//m_Light = new Light;
	//if (!m_Light)
	//{
	//	return false;
	//}

	//// Initialize the light object.
	//m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	//m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	//m_Light->SetDirection(1.0f, 0.0f, 0.0f);


	return true;
}

void Graphics::Shutdown()
{
	// Release the color shader object.
	//COLOURSHADER
	/*if (m_ColourShader)
	{
		m_ColourShader->Shutdown();
		delete m_ColourShader;
		m_ColourShader = 0;
	}*/

	//Release Anttweak bar
	TwTerminate();

	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}


	// Release the model object.
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}
	// Release the Direct3D object.
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}
}

bool Graphics::Frame()
{
	bool result;

	float rotation = 0.0f;

	// Update the rotation variable each frame
	rotation += rotation_speed * ((float)XM_PI / 180.0f);
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	// Render the graphics scene.
	result = Render(rotation);

	if (!result)
	{
		return false;
	}
	return true;
}

void Graphics::MoveObject(string direction)
{
	/*switch ()
	{
	case :
		
	default:
		break;
	}*/
}

void Graphics::RotateCamera(float x, float y, float z)
{
}

void Graphics::MoveCamera(float x, float y, float z)
{

	//XMFLOAT3 cam_position = m_Camera->GetPosition;
	XMFLOAT3 new_Pos = m_Camera->GetPosition();
	new_Pos.x += x;
	new_Pos.y += y;
	new_Pos.z += z;

	m_Camera->SetPosition(new_Pos.x, new_Pos.y, new_Pos.z);
}

bool Graphics::Render(float _rotation)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	
	// Clear the buffers to begin the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// Rotate the world matrix by the rotation value so that the triangle will spin.	
	worldMatrix = XMMatrixRotationY(_rotation);
	
	//Draw AntTweakBar
	TwDraw();

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(m_Direct3D->GetDeviceContext());

	/// Render the model using the colour shader.
	/*
	m_ColourShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}*/

	// Render the model using the light shader.
	/*result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor());
	if (!result)
	{
		return false;
	}*/


	// Render the model using the texture shader.
	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetVertexCount(), m_Model->GetInstanceCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture());
	if (!result)
	{
		return false;
	}

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}
