#pragma once

#include "Component.h"

#pragma comment( lib, "Devil/libx86/DevIL.lib" )
#include "Devil\include\ilu.h"
#pragma comment( lib, "Devil/libx86/ILU.lib" )
#include "Devil\include\ilut.h"
#pragma comment( lib, "Devil/libx86/ILUT.lib" )


class MaterialComponent: public Component
{
public :
	MaterialComponent();
	~MaterialComponent();

public:
	int width = 0;
	int height = 0;
	int bpp = 0;

	std::string name;
	std::string path;

	GLuint bufferTexture = 0;
	ILubyte* dataTexture = nullptr;

	uint size = 0;
	char* materialBuffer = 0;
	bool useChecker = false;
};

