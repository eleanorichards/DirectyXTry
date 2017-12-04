#ifndef _TEXTURESHADERCLASS_H_
#define _TEXTURESHADERCLASS_H_


//This class will be used to draw the 3D models using vertex and pixel shaders.
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>
using namespace DirectX;
using namespace std;



// Class name: TextureShaderClass
class TextureShader
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

public:
	TextureShader();
	TextureShader(const TextureShader&);
	~TextureShader();

	bool Initialise(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*);

private:
	bool InitialiseShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*);
	void RenderShader(ID3D11DeviceContext*, int, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	//There is a new private variable for the sampler state pointer.This pointer will be used to interface with the texture shader.

		ID3D11SamplerState* m_sampleState;
};

#endif