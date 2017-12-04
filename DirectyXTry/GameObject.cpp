#include "stdafx.h"
#include "GameObject.h"

#include "AntTweakBar.h"

float modelRotSpeed = 0.1f;
//TwInit(TW_DIRECT3D11, myD3DDevice); // for Direct3D 11

GameObject::GameObject()
{
	//create antTweakbars
	TwBar* bTweakBar;
	bTweakBar = TwNewBar("Model Info");
	TwAddVarRW(bTweakBar, "Rotation Speed", TW_TYPE_FLOAT, &modelRotSpeed, "min=0 max=10 step=0.1");

	//TwBar* bTweakBar;
	//bTweakBar = TwGetBarByName("Model Info");

}

GameObject::GameObject(const GameObject &)
{
}

GameObject::~GameObject()
{
}

void GameObject::SetPosition()
{
}
