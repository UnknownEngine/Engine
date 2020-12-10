#include "Globals.h"
#include "Application.h"
#include "ModuleResources.h"
#include "ModuleFSystem.h"
#include "MaterialComponent.h"
#include "Resource.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "MathGeoLib/include/MathGeoLibFwd.h"
ModuleResourceManager::ModuleResourceManager(Application* app, bool start_enabled): Module(app, start_enabled)
{
}

ModuleResourceManager::~ModuleResourceManager()
{
}

bool ModuleResourceManager::Start()
{
	fbxsPath = "Assets/FBXs/";
	texturesPath = "Assets/Textures/";
	texturesLibPath = "Library/Materials/";

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

int ModuleResourceManager::ImportFile(std::string file)
{
	JsonObj meta = CreateMaterialMetas(file);

	if (!meta.GetObjNames()==NULL)
	{
		std::string ext;
		App->fsystem->GetPathExtension(meta.GetString("Asset path"), &ext);

		switch (SetResourceType(ext.c_str()))
		{
		case ResourceType::texture:
			 ResourceTexture* newResource = new ResourceTexture(meta.GetInt("UID"), ResourceType::texture);

				break;
			
		}
	}
	return 0;
}

ResourceType ModuleResourceManager::SetResourceType(std::string extension)
{

	if (extension == "fbx" || extension == "FBX")
	{
		return ResourceType::mesh;
	}
	else if (extension == "png" || extension == "PNG" || extension == "tga" || extension == "TGA" || extension == "dae" || extension == "DAE" || extension == "dds" || extension == "DDS")
	{
		return ResourceType::texture;
	}
}

int ModuleResourceManager::GenerateNewUID()
{
	return LCG().Int();;
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
		ImportFile(texturesPathlist.at(i));
	}
}

JsonObj ModuleResourceManager::CreateMaterialMetas(std::string file)
{
	std::string realDir = texturesPath + file;
	std::string metaDir = App->fsystem->GetMetaPath(realDir);

	if (!App->fsystem->CheckIfExists(metaDir))
	{
		char* metaBuffer;
		JsonObj fileData;

		fileData.AddString("Asset path", realDir.c_str());
		fileData.AddInt("UID", GenerateNewUID());
		fileData.AddString("Library path", (texturesLibPath+std::to_string(fileData.GetInt("UID"))).c_str());

		uint metaSize = fileData.Save(&metaBuffer);
		App->fsystem->WriteFile(metaDir.c_str(), metaBuffer, metaSize);

		return fileData;
		//ReadMaterialMetas(fileData, file);
	}
	return JsonObj();
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

