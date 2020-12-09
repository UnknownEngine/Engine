#include "Globals.h"
#include "Application.h"
#include "ModuleResources.h"
#include "ModuleFSystem.h"
#include "MaterialComponent.h"
#include "Resource.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
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

int ModuleResourceManager::Find(const char* assets_file) const
{
	return 0;
}

int ModuleResourceManager::ImportFile(const char* new_assets_file)
{
	return 0;
}

int ModuleResourceManager::GenerateNewUID()
{

	return 0;
}

const Resource* ModuleResourceManager::RequestResource(int uid) const
{
	return nullptr;
}

Resource* ModuleResourceManager::RequestResource(int uid)
{
	return nullptr;
}

void ModuleResourceManager::ReleaseResource(int uid)
{
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

Resource* ModuleResourceManager::CreateNewResource(const char* assetsFile, ResourceType type)
{

	return nullptr;
}

