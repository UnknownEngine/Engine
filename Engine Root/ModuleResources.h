#pragma once
#include "Module.h"
#include "Globals.h"
#include "Json.h"
#include "Resource.h"
#include <vector>
#include <string>

class ModuleResourceManager :public Module {
public:
	ModuleResourceManager(Application* app, bool start_enabled = true);
	~ModuleResourceManager();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);

	int Find(const char* assets_file) const;
	int ImportFile(std::string file);


	ResourceType SetResourceType(std::string extension);
	int GenerateNewUID();

	const Resource* RequestResource(int uid) const;
	Resource* RequestResource(int uid);
	void ReleaseResource(int uid);


	bool CleanUp();

	void ImportTexturesAssets();
	JsonObj CreateMaterialMetas(std::string file);
	void ImportMeshAssets();

private:
	Resource* CreateNewResource(const char* assetsFile, ResourceType type);

public:
	std::string fbxsPath;
	std::string texturesPath;
	std::string texturesLibPath;

	std::vector<std::string> texturesPathlist;
	std::vector<std::string> FBXsPathlist;
	std::vector<JsonObj> textureMetas;
};


