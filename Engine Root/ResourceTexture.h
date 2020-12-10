#pragma once
#include "Resource.h"
#pragma comment( lib, "Devil/libx86/DevIL.lib" )
#include "Devil\include\ilu.h"
#pragma comment( lib, "Devil/libx86/ILU.lib" )
#include "Devil\include\ilut.h"
#pragma comment( lib, "Devil/libx86/ILUT.lib" )


class ResourceTexture :public Resource {

public:
	ResourceTexture(int UID, ResourceType type);
	~ResourceTexture();
	bool LoadToMemory() override;
	void Save(JsonObj& config) const override;
	void Load(JsonObj& config) override;
	ResourceType GetType() const override;

public:
	int width = 0;
	int height = 0;
	int bpp = 0;

	std::string name;
	std::string path;
	ResourceType TYPE;
	uint UID;

	GLuint bufferTexture = 0;
	ILubyte* dataTexture = nullptr;

	uint size = 0;
	char* materialBuffer = 0;
	bool useChecker = false;
};