#include "Globals.h"
#include "Application.h"
#include "ModuleResources.h"
#include "ModuleFSystem.h"
#include "MaterialComponent.h"
ModuleResourceManager::ModuleResourceManager(Application* app, bool start_enabled): Module(app, start_enabled)
{
}

ModuleResourceManager::~ModuleResourceManager()
{
}

bool ModuleResourceManager::Start()
{
	return true;
}

update_status ModuleResourceManager::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleResourceManager::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleResourceManager::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleResourceManager::CleanUp()
{
	return true;
}

void ModuleResourceManager::ImportTexturesAssets()
{
	std::vector<std::string> filesDirs;

	App->fsystem->DiscoverFiles("Assets/Textures", texturesPathlist, filesDirs);

	for (int i = 0; i < texturesPathlist.size(); i++)
	{
		char* buffer = "";
		App->fsystem->CreateMaterialMetas(buffer, texturesPathlist.at(i));
	}
}

void ModuleResourceManager::ImportMeshAssets()
{
	std::vector<std::string> filesDirs;

	App->fsystem->DiscoverFiles("Assets/FBXs", FBXsPathlist, filesDirs);

	for (int i = 0; i < FBXsPathlist.size(); i++)
	{
		char* buffer = "";
		App->fsystem->LoadFBXMeshes(FBXsPathlist.at(i),buffer);
	}
}
