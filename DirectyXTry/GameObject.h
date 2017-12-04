#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

// INCLUDES //
#include <d3d11.h>
#include <directxmath.h>
#include <d3dcompiler.h>
#include <fstream>
#include "Models.h"

class GameObject
{
private:

public:
	GameObject();
	GameObject(const GameObject&);
	~GameObject();

	void SetPosition();
	void SetRotation();

	void GetPosition();
	void GetRotation();


};

#endif