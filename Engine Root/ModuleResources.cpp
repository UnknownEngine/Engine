#include "Globals.h"
#include "Application.h"
#include "ModuleResources.h"
#include "ModuleFSystem.h"
#include "MaterialComponent.h"
#include "MeshComponent.h"
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
	modelsLibPath = "Library/Models/";

	ImportTexturesAssets();
	ImportMeshAssets();
	ImportMaterialsList();
	SetFbxList();
	SetTexturesList();


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
		{
			ResourceTexture* newResource = new ResourceTexture(meta.GetInt("UID"), ResourceType::texture);
			App->geometry->ImportTexture(meta, newResource);
			ret = newResource->UID;
			delete newResource;
			break;
		}
		case ResourceType::mesh:
		{
			App->geometry->ImportFBXMeshes(meta, realDir, metaDir);
			break;
		}
		default:
			break;
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

Resource* ModuleResourceManager::RequestResource(const char* path, ResourceType type)
{
	Resource* resource = nullptr;
	char* buffer;
	App->fsystem->ReadFile(path, &buffer);
	JsonObj meta(buffer);

	int uid = meta.GetInt("UID");

	std::map<int, Resource*>::iterator resourceIt = resourceMap.find(uid);
	if (resourceIt != resourceMap.end())
	{
		resourceIt->second->instances++;
		return resourceIt->second;
	}

	switch (type)
	{
	case texture:
		resource = LoadTexture(meta);
		break;
	case mesh:
		resource = LoadModel(uid);
		break;
	}
	//Cargar FBX ("Doble click) cargas .meta
	//encontrar UID en el .meta
	//Compruebo si está en el map sino lo está se crea de nuevo y se añade en el map
	//Si lo está se devuelve el resource

	return resource;
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
		realDir = fbxsPath + FBXsPathlist.at(i);
		metaDir = App->fsystem->GetMetaPath(realDir);
		if (!App->fsystem->CheckIfExists(metaDir))
		{
			ImportFile(FBXsPathlist.at(i), CreateMeshMetas(realDir, metaDir));
		}
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

JsonObj ModuleResourceManager::CreateMeshMetas(std::string realDir, std::string metaDir)
{
		JsonObj fileData;
		JsonArray meshArray;

		fileData.AddString("Asset Path", realDir.c_str());
		fileData.AddInt("UID", LCG().Int());
		fileData.AddString("Library path", ((modelsLibPath + std::to_string(fileData.GetInt("UID"))).c_str()));

	return fileData;
}


void ModuleResourceManager::SetFbxList()
{
	//std::vector<std::string> filesDirs;
	//App->fsystem->DiscoverFiles("Assets/FBXs", fbxList, filesDirs);
	App->fsystem->GetAllFilesWithExtension("Assets/FBXs", "fbx", fbxList);
	App->fsystem->GetAllFilesWithExtension("Assets/FBXs", "FBX", fbxList);
}

void ModuleResourceManager::SetTexturesList()
{
	//std::vector<std::string> filesDirs;
	//App->fsystem->DiscoverFiles("Assets/FBXs", fbxList, filesDirs);
	App->fsystem->GetAllFilesWithExtension("Assets/Textures", "tga", textureList);
	App->fsystem->GetAllFilesWithExtension("Assets/Textures", "png", textureList);
	App->fsystem->GetAllFilesWithExtension("Assets/Textures", "DAE", textureList);
	App->fsystem->GetAllFilesWithExtension("Assets/Textures", "mtl", textureList);
}

ResourceTexture* ModuleResourceManager::LoadTexture(JsonObj json)
{
	int uid = json.GetInt("UID");

	char* materialBuffer;
	uint size = App->fsystem->ReadFile((texturesLibPath + std::to_string(uid)).c_str(), &materialBuffer);

	ResourceTexture* r_texture = new ResourceTexture(uid, ResourceType::texture);

	App->geometry->CreateTextureBuffer(r_texture);
	App->geometry->LoadOurMaterial(materialBuffer, r_texture, size);
	//App->geometry->LoadTexturefromBuffer(uid,r_texture);
	App->geometry->LoadTexture(json.GetString("Asset path"), r_texture);
	r_texture->instances++;
	r_texture->UID = uid;
	resourceMap[uid] = r_texture;
	return r_texture;
}

ResourceTexture* ModuleResourceManager::LoadModel(int uid,GameObject* parent)
{
	char* buffer;
	uint size = App->fsystem->ReadFile((modelsLibPath + std::to_string(uid)).c_str(), &buffer);

	if (size <= 0) return nullptr;


	JsonObj modelMeta(buffer);
	int _uid = modelMeta.GetInt("UID");
	GameObject* newGameObject = new GameObject(modelMeta.GetString("Name"), uid);

	if (parent != nullptr) {
		newGameObject->ParentUID = parent->UID;
		newGameObject->parent = parent;
		parent->childs.push_back(newGameObject);
	}
	else
	{
		newGameObject->ParentUID = 0;
	}

	JsonArray arr_pos = modelMeta.GetArray("Position");
	JsonArray arr_rot = modelMeta.GetArray("Rotation");
	JsonArray arr_scale = modelMeta.GetArray("Scale");

	float3 position = arr_pos.GetFloat3(0);
	Quat rotation = arr_rot.GetQuaternion(0);
	float3 scale = arr_scale.GetFloat3(0);

	TransformComponent* c_component = new TransformComponent(position, rotation, scale);
	newGameObject->AddComponent(c_component);
	
	int mesh_uid = modelMeta.GetInt("Mesh UID");
	if (mesh_uid != 0) {

		ResourceMesh* r_mesh = nullptr;
		std::map<int, Resource*>::iterator resourceIt = resourceMap.find(mesh_uid);
		if (resourceIt != resourceMap.end())
		{
			resourceIt->second->instances++;
			r_mesh = static_cast<ResourceMesh*>(resourceIt->second);
		}
		else {

			r_mesh = new ResourceMesh(mesh_uid, ResourceType::mesh);
			r_mesh->instances++;
			resourceMap[mesh_uid] = r_mesh;
		}
		


		MeshComponent* c_mesh = new MeshComponent;
		MaterialComponent* c_material = new MaterialComponent;
		App->fsystem->ReadFile((meshesLibPath + std::to_string(mesh_uid)).c_str(), &r_mesh->meshBuffer);
		App->geometry->LoadOurMesh(r_mesh->meshBuffer, r_mesh);

		c_mesh->r_mesh = r_mesh;
		c_mesh->r_mesh->CreateAABB();
		c_mesh->UID = mesh_uid;

		App->geometry->CreateBuffer(c_mesh->r_mesh);

		newGameObject->AddComponent(c_mesh);
		newGameObject->AddComponent(c_material);


	}
	if (parent == nullptr)
	{
		App->scene_intro->gameObjectsList.push_back(newGameObject);
	}
	JsonArray childs = modelMeta.GetArray("Childs UID");

	if (modelMeta.GetArray("Childs UID") != NULL) {
		std::vector<int> childs_uid = childs.GetUIDs(0);
		if (childs_uid.size() > 0) {
			// HAS MODEL CHILDS
			for (uint i = 0; i < childs_uid.size(); i++)
			{
				LoadModel(childs_uid.at(i), newGameObject);
			}
		}
	}
	return nullptr;
}
