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
	meshesLibPath= "Library/Meshes/";

	ImportTexturesAssets();
	ImportMaterialsList();
	
	LoadResource();

	return true;
}

update_status ModuleResourceManager::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleResourceManager::Update(float dt)
{
	//ImportMeshAssets();
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

int ModuleResourceManager::ImportFile(std::string file, JsonObj meta)
{
	int ret = 0;

	if (!meta.GetObjNames()==NULL)
	{
		std::string ext;
		App->fsystem->GetPathExtension(file.c_str(), &ext);

		switch (SetResourceType(ext.c_str()))
		{
		case ResourceType::texture:
			ResourceTexture* newResource = new ResourceTexture(meta.GetInt("UID"), ResourceType::texture);
			App->geometry->ImportTexture(meta, newResource);
			ret = newResource->UID;
			delete newResource;
			break;
		//case ResourceType::mesh:
		//	//ResourceMesh* newmeshResource = new ResourceMesh(meta.GetInt("UID"), ResourceType::mesh);
		//	break;

		}
		
	}
	return ret;
}

ResourceType ModuleResourceManager::SetResourceType(std::string extension)
{

	if (extension == "fbx" || extension == "FBX")
	{
		return ResourceType::mesh;
	}
	else if (extension == "png" || extension == "PNG" || extension == "tga" || extension == "TGA" || extension == "dae" || extension == "DAE" || extension == "dds" || extension == "DDS" || extension == "mtl" || extension == "MTL")
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
		realDir=texturesPath + texturesPathlist.at(i);
		metaDir=App->fsystem->GetMetaPath(realDir);

		if (!App->fsystem->CheckIfExists(metaDir))
		{
			ImportFile(texturesPathlist.at(i), CreateMaterialMetas(realDir,metaDir));
		}
	}
}

void ModuleResourceManager::ImportMaterialsList()
{
	std::vector<std::string> filesDirs;
	App->fsystem->DiscoverFiles("Library/Materials", materialsList, filesDirs);

}


void ModuleResourceManager::ImportMeshAssets()
{
	std::vector<std::string> filesDirs;
	App->fsystem->DiscoverFiles("Assets/FBXs", FBXsPathlist, filesDirs);

	for (int i = 0; i < FBXsPathlist.size(); i++)
	{
		//ImportFile(FBXsPathlist.at(i), CreateMaterialMetas(FBXsPathlist.at(i)));
	}
}


JsonObj ModuleResourceManager::CreateMaterialMetas(std::string realDir, std::string metaDir)
{
		char* metaBuffer;
		JsonObj fileData;

		fileData.AddString("Asset path", realDir.c_str());
		fileData.AddInt("UID", GenerateNewUID());
		fileData.AddString("Library path", (texturesLibPath + std::to_string(fileData.GetInt("UID"))).c_str());

		uint metaSize = fileData.Save(&metaBuffer);
		App->fsystem->WriteFile(metaDir.c_str(), metaBuffer, metaSize);

		return fileData;
}

JsonObj ModuleResourceManager::CreateMeshMetas(std::string file)
{
	bool createMeshesArray = true;

	std::string realDir = fbxsPath + file;
	std::string metaDir = App->fsystem->GetMetaPath(realDir);


	if (!App->fsystem->CheckIfExists(metaDir))
	{
		JsonObj fileData;
		JsonArray meshArray;

		fileData.AddString("Asset Path", realDir.c_str());
		fileData.AddInt("UID", LCG().Int());

		//std::string libPath = "Library/Models/";
		//libPath += std::to_string(fileData.GetInt("UID"));
		////fileData.AddString("Library path:", libPath.c_str());

}
	return JsonObj();
}


ResourceTexture* ModuleResourceManager::LoadResource()
{
	char* buffer;
	App->fsystem->ReadFile("Assets/Textures/Baker_house.mta", &buffer);

	JsonObj meta(buffer);

	int uid = meta.GetInt("UID");
	LOG("%i", uid);

	char* materialBuffer;
	uint size = App->fsystem->ReadFile(meta.GetString("Library path"), &materialBuffer);

	ResourceTexture* r_texture = new ResourceTexture(uid, ResourceType::texture);

	App->geometry->LoadOurMaterial(materialBuffer, r_texture, size);

	return r_texture;

}