#pragma once

#include "Globals.h"
#include "Json.h"
#include <vector>
#include <string>



enum ResourceType {
	texture,
	mesh,
	unknown,
};
class ResourceTexture;
class Resource {
public:
	Resource(int UID, ResourceType type);
	~Resource();

	virtual ResourceType GetType() const;
	int GetUID() const;

	const char* GetAssetFile() const;
	const char* GetLibraryFile() const;

	bool IsLoadedToMemory() const;
	virtual bool LoadToMemory();

	virtual void Save(JsonObj& config) const;
	virtual void Load(JsonObj& config);

protected:
	int UID = 0;
	std::string fileOnAssets;
	std::string fileOnLibrary;

	ResourceType type = unknown;
};
