#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include "d3dclass.h"
#include "camera.h"
#include "models.h"
#include "TextureShader.h"
#include "LightShader.h"
#include "Light.h"
#include "Input.h"
//#include "ColourShader.h"

// GLOBALS //
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class Graphics
{
public:
	Graphics();
	Graphics(const Graphics&);
	~Graphics();

	bool Initialise(int, int, HWND, HINSTANCE);
	void Shutdown();
	bool Frame();
	void MoveObject(string direction);
	void RotateCamera(float x, float y, float z);
	void MoveCamera(float x, float y, float z);
	void ResetModel();
	bool Render(float _rotation);
private:

	void TestIntersection(float, float);
	bool RaySphereIntersect(XMFLOAT3, XMFLOAT3, float);
	bool HandleInput();
	void takeInput();
	
private:

	float rotation_speed = 0.1f;
	float rotation = 0.0f;
	float mouseX;
	float mouseY;
	float screen_width;
	float screen_height;
	bool  export_model = false;
	bool begin_check = false;
	bool over_object = false;
	bool reset_model = false;
	string modelFileName = "";
	string textureFileName = "";
private:
	D3DClass* m_Direct3D;
	Camera* m_Camera;
	Models* m_Model;
	TextureShader* m_TextureShader;
	LightShader* m_LightShader;
	Light* m_Light;
	Input* m_Input;
	//ColourShader* m_ColourShader;
};

#endif