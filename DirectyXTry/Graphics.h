#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include "d3dclass.h"
#include "camera.h"
#include "models.h"
//#include "ColourShader.h"
#include "TextureShader.h"
#include "LightShader.h"
#include "Light.h"

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

	bool Initialise(int, int, HWND);
	void Shutdown();
	bool Frame();
	void MoveObject(string direction);
	void RotateCamera(float x, float y, float z);
	void MoveCamera(float x, float y, float z);
	bool Render(float _rotation);

private:
	float rotation_speed = 0.1f;
private:
	D3DClass* m_Direct3D;
	Camera* m_Camera;
	Models* m_Model;
	//ColourShader* m_ColourShader;
	TextureShader* m_TextureShader;
	LightShader* m_LightShader;
	Light* m_Light;

};

#endif