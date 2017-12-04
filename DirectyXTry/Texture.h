//The TextureClass encapsulates the loading, unloading, and accessing of a single texture resource. For each texture needed an object of this class must be instantiated.

#ifndef _TEXTURECLASS_H_
#define _TEXTURECLASS_H_

// INCLUDES //
#include <d3d11.h>
#include <stdio.h>


// Class name: TextureClass
class Texture
{
private:
	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

public:
	Texture();
	Texture(const Texture&);
	~Texture();

	bool Initialise(ID3D11Device*, ID3D11DeviceContext*, char*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();
private:
	bool LoadTarga(char*, int&, int&);

private:
	unsigned char* m_targaData;
	ID3D11Texture2D* m_texture;
	ID3D11ShaderResourceView* m_textureView;
};

#endif