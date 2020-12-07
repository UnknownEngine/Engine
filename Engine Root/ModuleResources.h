#pragma once
#include "Module.h"
#include "Globals.h"
#include "Json.h"
#include <vector>
#include <string>

class ModuleResourceManager :public Module {
public:
	ModuleResourceManager(Application* app, bool start_enabled = true);
	~ModuleResourceManager();

	bool Start();
	update_status PreUpdate(float dt) override;
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void ImportTexturesAssets();
	void ImportMeshAssets();


public:
	std::vector<std::string> texturesPathlist;
	std::vector<std::string> FBXsPathlist;
	std::vector<JsonObj> textureMetas;
};
