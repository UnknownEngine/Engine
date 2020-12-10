#pragma once

#include "Component.h"
#pragma comment( lib, "Devil/libx86/DevIL.lib" )
#include "Devil\include\ilu.h"
#pragma comment( lib, "Devil/libx86/ILU.lib" )
#include "Devil\include\ilut.h"
#pragma comment( lib, "Devil/libx86/ILUT.lib" )

#include "ResourceTexture.h"


class MaterialComponent: public Component
{
public :
	MaterialComponent();
	~MaterialComponent();
public:

	uint UID;

	ResourceTexture* r_texture = nullptr;
};

