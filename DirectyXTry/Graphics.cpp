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
	m_Input = 0;
	//COLOURSHADER
	//m_ColourShader = 0;	

}

Graphics::Graphics(const Graphics &)
{
}

Graphics::~Graphics()
{
}

//ANTTWEAAK callback bbutton for exporting model
void TW_CALL RunCBExport(void *_Model)
{
	Models* m_Model;
	m_Model = (Models*)_Model;
	m_Model->ExportModel();
	m_Model = 0;
	delete m_Model;
}

bool Graphics::Initialise(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	screen_height = screenHeight;
	screen_width = screenWidth;

	// Initialize the input object.
	//m_Input = new Input;
	//result = m_Input->Initialise(m_hinstance, hwnd, screenWidth, screenHeight);
	//if (!result)
	//{
	//	MessageBox(hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
	//	return false;
	//}


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
	result = m_Model->Initialise(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(),"../DirectyXTry/Assets/tri.txt", "../DirectyXTry/Assets/Stone03.tga");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}


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
	TwAddVarRO(bTweakBar, "MouseX", TW_TYPE_FLOAT, &mouseX, "step=0.1");
	TwAddVarRO(bTweakBar, "MouseY", TW_TYPE_FLOAT, &mouseY, "step=0.1");
	TwAddButton(bTweakBar, "Export Model", RunCBExport, m_Model, "label='export'");
	TwAddVarRO(bTweakBar, "Mouse Over", TW_TYPE_BOOLCPP, &over_object, "");
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
	//release the input object 
	if (m_Input)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input = 0;
	}
}

bool Graphics::Frame(float _mouseX, float _mouseY)
{
	mouseX = _mouseX;
	mouseY = _mouseY;
	bool result;

	// Update the rotation variable each frame
	rotation += rotation_speed * ((float)XM_PI / 180.0f);
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	result = HandleInput();
	if (!result)
	{
		return false;
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

//	XMFLOAT3 cam_position = m_Camera->GetPosition;
	XMFLOAT3 new_Pos = m_Camera->GetPosition();
	new_Pos.x += x;
	new_Pos.y += y;
	new_Pos.z += z;

	m_Camera->SetPosition(new_Pos.x, new_Pos.y, new_Pos.z);
}

bool Graphics::Render(float _rotation)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, translateMatrix, inverseWorldMatrix;

	bool result;
	
	// Clear the buffers to begin the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// Translate to the location of the model.	//FLOATS SHOULD BE EQUAL TO MODEL OFFSET
	translateMatrix = XMMatrixTranslation(-5.0f, 1.0f, 5.0f);
	worldMatrix = XMMatrixMultiply(worldMatrix, translateMatrix);
	inverseWorldMatrix = XMMatrixInverse(NULL, worldMatrix);

	//reset world matrix after all that multiplying
	m_Direct3D->GetWorldMatrix(worldMatrix);

	// Rotate the world matrix by the rotation value so that the triangle will spin.	
	worldMatrix = XMMatrixRotationY(_rotation);
	
	//Draw AntTweakBar
	TwDraw();

	// Put the model buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(m_Direct3D->GetDeviceContext());

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

void Graphics::TestIntersection(float mouseX, float mouseY)
{
	float pointX, pointY;
	XMMATRIX projectionMatrix, viewMatrix, inverseViewMatrix, worldMatrix, translateMatrix, inverseWorldMatrix;	
	XMFLOAT3X3 inverseViewMatrixFloat, projectionMatrixFloat;
	XMFLOAT3 origin, direction;
	XMFLOAT3 rayOriginFloat, rayDirFloat, directionFloat;
	XMVECTOR rayDir,  rayOrigin;
	XMVECTOR  originVector, directionVector;
	bool intersect, result;
	
	//Normalise mouse cursor coords (between -1, 1)
	pointX = ((2.0f * mouseX) / screen_width) - 1.0f;
	pointY = (((2.0f * mouseY) / screen_height) - 1.0f) * -1.0f;

	// Adjust the points using the projection matrix to account for the aspect ratio of the viewport.
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	//these aren't right
	
	XMStoreFloat3x3(&projectionMatrixFloat, projectionMatrix);
	
	//HERE
	//pointX /= projectionMatrixFloat._11 + inverseViewMatrixFloat._21 + inverseViewMatrixFloat._31;
	//pointY /= projectionMatrixFloat.operator();

	// Get the inverse of the view matrix.
	m_Camera->GetViewMatrix(viewMatrix);
	inverseViewMatrix = XMMatrixInverse(NULL, viewMatrix);
	//convert to float4x4 for maths
	XMStoreFloat3x3(&inverseViewMatrixFloat, inverseViewMatrix);
	// Calculate the direction of the picking ray in view space.
	direction.x = (pointX * inverseViewMatrixFloat._11) + (pointY * inverseViewMatrixFloat._21) + inverseViewMatrixFloat._31;
	direction.y = (pointX * inverseViewMatrixFloat._12) + (pointY * inverseViewMatrixFloat._22) + inverseViewMatrixFloat._32;
	direction.z = (pointX * inverseViewMatrixFloat._13) + (pointY * inverseViewMatrixFloat._23) + inverseViewMatrixFloat._33;

	// Get the origin of the picking ray which is the position of the camera.
	origin = m_Camera->GetPosition();

	// Get the world matrix and translate to the location of the sphere.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	translateMatrix = XMMatrixTranslation(0.0f,0.0f,0.0f);
	XMMatrixMultiply(worldMatrix, translateMatrix);

	// Now get the inverse of the translated world matrix.
	inverseWorldMatrix = XMMatrixInverse(NULL,  worldMatrix);
	//convert any float3s to vectors that we need
	originVector = XMLoadFloat3(&origin);
	directionVector = XMLoadFloat3(&direction);
	// Now transform the ray origin and the ray direction from view space to world space.
	rayOrigin = XMVector3TransformCoord(originVector, inverseWorldMatrix);
	rayDir = XMVector3TransformNormal(directionVector, inverseWorldMatrix);
	// Normalize the ray direction.
	XMVector3Normalize(rayDir);
	//convert vectors to floats for maths...
	XMStoreFloat3(&rayOriginFloat, rayOrigin);
	XMStoreFloat3(&rayDirFloat, rayDir);
	// Now perform the ray-sphere intersection test.
	intersect = RaySphereIntersect(rayOriginFloat, rayDirFloat, 1.0f);

	if (intersect == true)
	{
		over_object = true;
	}
	else
	{
		over_object = false;		
	}

	return;
}

bool Graphics::RaySphereIntersect(XMFLOAT3 rayOrigin, XMFLOAT3 rayDirection, float radius)
{
	float a, b, c, discriminant;


	// Calculate the a, b, and c coefficients.
	a = (rayDirection.x * rayDirection.x) + (rayDirection.y * rayDirection.y) + (rayDirection.z * rayDirection.z);
	b = ((rayDirection.x * rayOrigin.x) + (rayDirection.y * rayOrigin.y) + (rayDirection.z * rayOrigin.z)) * 2.0f;
	c = ((rayOrigin.x * rayOrigin.x) + (rayOrigin.y * rayOrigin.y) + (rayOrigin.z * rayOrigin.z)) - (radius * radius);

	// Find the discriminant.
	discriminant = (b * b) - (4 * a * c);

	// if discriminant is negative the picking ray missed the sphere, otherwise it intersected the sphere.
	if (discriminant < 0.0f)
	{
		return false;
	}

	return true;
}

bool Graphics::HandleInput()
{
	bool result;
	float mouseX, mouseY;
	/*result = m_Input->Frame();
	if (!result)
	{
		return false;
	}*/

	// Check if the left mouse button has been pressed.
	if (m_Input->IsLeftMouseButtonDown())
	{
		// If they have clicked on the screen with the mouse then perform an intersection test.
		if (begin_check == false)
		{
			begin_check = true;
			m_Input->GetMouseLocation(mouseX, mouseY);
			TestIntersection(mouseX, mouseY);
		}
	}

	// Check if the left mouse button has been released.
	if (!m_Input->IsLeftMouseButtonDown())
	{
		begin_check = false;
	}
	return  true;
}

 
