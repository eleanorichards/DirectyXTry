
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

// INCLUDES //
#include <d3d11.h>
#include <directxmath.h>
#include "Texture.h"
#include <fstream>
using namespace std;
using namespace DirectX;

// Class name: ModelClass
class Models
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;		
		XMFLOAT3 normal;
		XMFLOAT2 texture;
	};

	struct InstanceType
	{
		XMFLOAT3 position;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	Models();
	Models(const Models&);
	~Models();
	bool Initialise(ID3D11Device*, ID3D11DeviceContext*, char *, char*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);
	bool ExportModel();
	int GetVertexCount();
	int GetInstanceCount();

	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitialiseBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext *deviceContext,  char*);
	void ReleaseTexture();
	bool LoadModel(char*);
	void ReleaseModel();

private:
	ID3D11Buffer *m_vertexBuffer, *m_instanceBuffer;
	int m_vertexCount;
	int m_instanceCount;
	Texture* m_Texture;
	ModelType* m_model;
};

#endif
