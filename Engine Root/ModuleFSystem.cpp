#include "Globals.h"
#include "ModuleFSystem.h"
#include "Application.h"

#include "PhysFS/include/physfs.h"
#include <fstream>
#include <filesystem>

#include "Assimp/include/cfileio.h"
#include "Assimp/include/types.h"
#include "SDL/include/SDL.h"
#include "MeshComponent.h"
#include "MaterialComponent.h"
#include "TransformComponent.h"
#include "Json.h"

#pragma comment( lib, "PhysFS/libx86/physfs.lib" )

M_FileSystem::M_FileSystem(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	// needs to be created before Init so other modules can use it
	char* base_path = SDL_GetBasePath();
	PHYSFS_init(nullptr);
	SDL_free(base_path);

	//Setting the working directory as the writing directory
	if (PHYSFS_setWriteDir(".") == 0)
		LOG("File System error while creating write dir: %s\n", PHYSFS_getLastError());

	AddPath("."); //Adding ProjectFolder (working directory)
	AddPath("Assets");
	CreateLibraryDirectories();
}

// Destructor
M_FileSystem::~M_FileSystem()
{
	PHYSFS_deinit();
}

// Called before render is available
bool M_FileSystem::Init()
{
	LOG("Loading File System");
	bool ret = true;

	// Ask SDL for a write dir

	// Trun this on while in game mode
	//if(PHYSFS_setWriteDir(write_path) == 0)
	//	LOG("File System error while creating write dir: %s\n", PHYSFS_getLastError());

	return ret;
}

// Called before quitting
bool M_FileSystem::CleanUp()
{
	//LOG("Freeing File System subsystem");

	return true;
}

void M_FileSystem::CreateLibraryDirectories()
{
	CreateDir(LIBRARY_PATH);
	CreateDir(MESHES_PATH);
	CreateDir(TEXTURES_PATH);
}

// Add a new zip file or folder
bool M_FileSystem::AddPath(const char* path_or_zip)
{
	bool ret = false;

	if (PHYSFS_mount(path_or_zip, nullptr, 1) == 0)
	{
		LOG("File System error while adding a path or zip: %s\n", PHYSFS_getLastError());
	}
	else
		ret = true;

	return ret;
}

// Check if a file exists
bool M_FileSystem::Exists(const char* file) const
{
	return PHYSFS_exists(file) != 0;
}

uint M_FileSystem::WriteFile(const char* file, char* buffer, uint size)
{

	uint ret = 0;
	bool overwrite = PHYSFS_exists(file) != 0;
	PHYSFS_file* fs_file= PHYSFS_openWrite(file);
	const char* writedir = PHYSFS_getWriteDir();
	if (fs_file != nullptr)
	{
		uint written = (uint)PHYSFS_write(fs_file, (const void*)buffer, 1, size);

		if (written != size)
		{
			LOG("Couldn't write file");
		}
		else
		{
			LOG("File written");
			ret = written;
		}	

		if (PHYSFS_close(fs_file) == 0)
			LOG("[error] File System error while closing file %s: %s", file, PHYSFS_getLastError());
	}
	else
	{
		LOG("Error while opening file");
	}

	return ret;
}

uint M_FileSystem::ReadFile(const char* file, char** buffer)
{
	uint ret = 0;

	PHYSFS_file* fs_file = PHYSFS_openRead(file);
	if (fs_file != nullptr)
	{
		PHYSFS_sint32 size = (PHYSFS_sint32)PHYSFS_fileLength(fs_file);

		if (size > 0)
		{
			*buffer = new char[size + 1];
			uint readed = (uint)PHYSFS_read(fs_file, *buffer, 1, size);
			if (readed != size)
			{
				LOG("File System error while reading from file %s: %s\n", file, PHYSFS_getLastError());
				RELEASE_ARRAY(buffer);
			}
			else
			{
				ret = readed;
				//Adding end of file at the end of the buffer. Loading a shader file does not add this for some reason
				(*buffer)[size] = '\0';
			}
		}

		if (PHYSFS_close(fs_file) == 0)
			LOG("File System error while closing file %s: %s\n", file, PHYSFS_getLastError());
	}
	else
		LOG("File System error while opening file %s: %s\n", file, PHYSFS_getLastError());

	return ret;
}

bool M_FileSystem::CreateDir(const char* dir)
{
	if (IsDirectory(dir) == false)
	{
		PHYSFS_mkdir(dir);
		return true;
	}
	return false;
}

// Check if a file is a directory
bool M_FileSystem::IsDirectory(const char* file) const
{
	return PHYSFS_isDirectory(file) != 0;
}

const char * M_FileSystem::GetWriteDir() const
{
	//TODO: erase first annoying dot (".")
	return PHYSFS_getWriteDir();
}

void M_FileSystem::DiscoverFiles(const char* directory, std::vector<std::string> & file_list, std::vector<std::string> & dir_list) const
{
	char **rc = PHYSFS_enumerateFiles(directory);
	char **i;

	for (i = rc; *i != nullptr; i++)
	{
		std::string str = std::string(directory) + std::string("/") + std::string(*i);
		if (IsDirectory(str.c_str()))
			dir_list.push_back(*i);
		else
			file_list.push_back(*i);
	}

	PHYSFS_freeList(rc);
}

void M_FileSystem::GetAllFilesWithExtension(const char* directory, const char* extension, std::vector<std::string>& file_list) const
{
	std::vector<std::string> files;
	std::vector<std::string> dirs;
	DiscoverFiles(directory, files, dirs);

	for (uint i = 0; i < files.size(); i++)
	{
		std::string ext;
		SplitFilePath(files[i].c_str(), nullptr, nullptr, &ext);

		if (ext == extension)
			file_list.push_back(files[i]);
	}
}

void M_FileSystem::GetRealDir(const char* path, std::string& output) const
{
	output = PHYSFS_getBaseDir();

	std::string baseDir = PHYSFS_getBaseDir();
	std::string searchPath = *PHYSFS_getSearchPath();
	std::string realDir = PHYSFS_getRealDir(path);

	output.append(*PHYSFS_getSearchPath()).append("/");
	output.append(PHYSFS_getRealDir(path)).append("/").append(path);
}

std::string M_FileSystem::GetPathRelativeToAssets(const char* originalPath) const
{
	std::string ret;
	GetRealDir(originalPath, ret);

	return ret;
}

bool M_FileSystem::HasExtension(const char* path) const
{
	std::string ext = "";
	SplitFilePath(path, nullptr, nullptr, &ext);
	return ext != "";
}

bool M_FileSystem::HasExtension(const char* path, std::string extension) const
{
	std::string ext = "";
	SplitFilePath(path, nullptr, nullptr, &ext);
	return ext == extension;
}

bool M_FileSystem::HasExtension(const char* path, std::vector<std::string> extensions) const
{
	std::string ext = "";
	SplitFilePath(path, nullptr, nullptr, &ext);
	if (ext == "")
		return true;
	for (uint i = 0; i < extensions.size(); i++)
	{
		if (extensions[i] == ext)
			return true;
	}
	return false;
}

std::string M_FileSystem::NormalizePath(const char * full_path) const
{
	std::string newPath(full_path);
	for (int i = 0; i < newPath.size(); ++i)
	{
		if (newPath[i] == '\\')
			newPath[i] = '/';
	}
	return newPath;
}

void M_FileSystem::SplitFilePath(const char * full_path, std::string * path, std::string * file, std::string * extension) const
{
	if (full_path != nullptr)
	{
		std::string full(full_path);
		size_t pos_separator = full.find_last_of("\\/");
		size_t pos_dot = full.find_last_of(".");

		if (path != nullptr)
		{
			if (pos_separator < full.length())
				*path = full.substr(0, pos_separator + 1);
			else
				path->clear();
		}

		if (file != nullptr)
		{
			if (pos_separator < full.length())
				*file = full.substr(pos_separator + 1, pos_dot - pos_separator - 1);
			else
				*file = full.substr(0, pos_dot);
		}

		if (extension != nullptr)
		{
			if (pos_dot < full.length())
				*extension = full.substr(pos_dot + 1);
			else
				extension->clear();
		}
	}
}

void M_FileSystem::DetectExtension(std::string path, std::string file, std::string extension) const
{
	
	if (extension == "fbx" || extension == "FBX")
	{
		std::string fileWithExtension = file + "." + extension;
		std::string realName = "/FBXs/" + fileWithExtension;

		char* buffer = nullptr;
		std::string realDir = PHYSFS_getRealDir(realName.c_str());
		realDir += realName;

		uint size =Load(realDir.c_str(), &buffer);
		if (buffer != nullptr)
		{
			App->geometry->LoadFbx(buffer,size,file,file);
		}
	}
	else if (extension == "png" || extension == "PNG")
	{
		char* buffer = nullptr;
		std::string realName = "Assets/Textures/" + file + "." + extension;
	
		uint size = Load(realName.c_str(), &buffer);
		GameObject* gameObject = App->scene_intro->selected;
		if (gameObject == nullptr) return;
		App->geometry->LoadTexture(realName.c_str(), gameObject->GetMaterialComponent());

	}
}

void M_FileSystem::CreatePrimitives(std::string path, std::string file)
{
	char* buffer = nullptr;
	
	uint size = Load(path.c_str(), &buffer);
	if (buffer != nullptr)
	{
		App->geometry->LoadFbx(buffer, size, file, path);
	}
}

void M_FileSystem::SaveScene(char** sceneBuffer)
{
	JsonObj scene;
	if (!App->scene_intro->gameObjectsList.empty())
	{
		scene.AddArray("GameObjects");
		SaveGameObjects(scene);
	}
	else
	{
		LOG("Any gameObjects to be saved");
	}
	uint size = scene.Save(sceneBuffer);
	App->fsystem->WriteFile("Library/tryingstuff2.json", *sceneBuffer, size);

}

JsonArray M_FileSystem::SaveGameObjects(JsonObj scene)
{
	JsonArray gameObjects = scene.GetArray("GameObjects");

	for (int it = 0; it < App->scene_intro->gameObjectsList.size(); it++)
	{
		JsonObj gameObject;
		gameObjects.AddObject(gameObject);
		gameObject.AddInt("UID", App->scene_intro->gameObjectsList.at(it)->UID);
		gameObject.AddString("Name", App->scene_intro->gameObjectsList.at(it)->nameID.c_str());
		gameObject.AddBool("Active", App->scene_intro->gameObjectsList.at(it)->active);
		if (!App->scene_intro->gameObjectsList.at(it)->childs.empty())
		{
			SaveGobjsChilds(App->scene_intro->gameObjectsList.at(it), gameObject);
		}
		if (!App->scene_intro->gameObjectsList.at(it)->components.empty())
		{
			SaveGobjsComponentes(App->scene_intro->gameObjectsList.at(it), gameObject);
		}
	}
	return gameObjects;
}

void M_FileSystem::SaveGobjsChilds(GameObject* gameObject, JsonObj JsonGob)
{
	JsonArray childs = JsonGob.AddArray("Childs");
	for (int i = 0; i < gameObject->childs.size(); i++)
	{
		JsonObj child;
		childs.AddObject(child);
		child.AddInt("UID", gameObject->childs.at(i)->UID);
		child.AddInt("Parent UID", gameObject->childs.at(i)->ParentUID);
		child.AddString("Parent", gameObject->childs.at(i)->parent->nameID.c_str());
		child.AddString("Name", gameObject->childs.at(i)->nameID.c_str());
		child.AddBool("Active", gameObject->childs.at(i)->active);
		if (!gameObject->childs.at(i)->components.empty())
		{
			SaveGobjsComponentes(gameObject->childs.at(i), child);
		}
	}
}

void M_FileSystem::SaveGobjsComponentes(GameObject* gameObject, JsonObj JsonGob)
{
	JsonArray components = JsonGob.AddArray("Components");
	for (int i = 0; i < gameObject->components.size(); i++)
	{
		JsonObj component;
		components.AddObject(component);
		if (gameObject->components.at(i)->type == ComponentType::Mesh)
		{
			SaveMesh(component, gameObject);
		}
		if (gameObject->components.at(i)->type == ComponentType::Material)
		{
			SaveMaterial(component, gameObject);
		}
		if (gameObject->components.at(i)->type == ComponentType::Transform)
		{
			SaveTransform(component, gameObject);
		}
	}
}

void M_FileSystem::LoadScene(char* sceneBuffer)
{
	App->scene_intro->gameObjectsList.clear();
	ReadFile("Library/tryingstuff2.json",&sceneBuffer);
	JsonObj* myScene = new JsonObj(sceneBuffer);

	JsonArray gameObjects= myScene->GetArray("GameObjects");
	for (int i = 0; i < gameObjects.Size(); ++i)
	{
		JsonObj object = gameObjects.GetObjectAt(i);
		
		GameObject* created_gameobject = LoadGameObjects(object);

		if (object.GetArray("Childs") != NULL)
		{
			LoadGobjsChilds(created_gameobject, object);
		}
		if (object.GetArray("Components") != NULL)
		{
			LoadGobjsComponents(created_gameobject, object);
		}
		App->scene_intro->gameObjectsList.push_back(created_gameobject);
	}
}

GameObject* M_FileSystem::LoadGameObjects(JsonObj current_node)
{
	GameObject* gameObject = new GameObject(current_node.GetString("Name"));

	gameObject->UID = current_node.GetInt("UID");
	gameObject->active = current_node.GetBool("Active");

	return gameObject;
}

void M_FileSystem::LoadGobjsChilds(GameObject* gameObject, JsonObj current_node)
{
	JsonArray childs_array=current_node.GetArray("Childs");
	JsonObj childs_iterator;
	for (int i = 0; i < childs_array.Size(); ++i)
	{
		childs_iterator = childs_array.GetObjectAt(i);

		GameObject* newGameObject = new GameObject(childs_iterator.GetString("Name"),gameObject);
		newGameObject->UID = childs_iterator.GetInt("UID");
		newGameObject->ParentUID = childs_iterator.GetInt("Parent UID");
		newGameObject->active = childs_iterator.GetBool("Active");

		if (childs_iterator.GetArray("Components") != NULL)
		{
			LoadGobjsComponents(newGameObject, childs_iterator);
		}
		gameObject->childs.push_back(newGameObject);
	}
}

void M_FileSystem::LoadGobjsComponents(GameObject* gameObject, JsonObj current_node)
{
	JsonArray components_array = current_node.GetArray("Components");
	JsonObj components_iterator;
	for (int i = 0; i < components_array.Size(); ++i)
	{
		components_iterator = components_array.GetObjectAt(i);

			std::string type = components_iterator.GetString("TYPE");
			if (type=="Mesh")
			{
				LoadMesh(components_iterator, gameObject);
			}
			else if (type == "Material")
			{
				LoadMaterial(components_iterator, gameObject);
			}
			else if (type == "Transform")
			{
				LoadTransform(components_iterator, gameObject);
			}
	}
}

//  ----------------------------------------------------------------------------------------------
//  --------------------------------------------COMPONENTS SAVING----------------------------------
//  ----------------------------------------------------------------------------------------------

void M_FileSystem::SaveMesh(JsonObj component, GameObject* gameObject)
{
	MeshComponent* mesh = gameObject->GetMeshComponent();

	component.AddInt("UID", mesh->UID);
	component.AddString("TYPE", "Mesh");
	component.AddString("Name", mesh->name.c_str());
	component.AddString("Path", mesh->path.c_str());

	component.AddInt("Num vertices", mesh->num_vertices);
	//component.AddInt("ID Vertices", mesh->id_vertices);

	component.AddInt("Num indices", mesh->num_indices);
	//component.AddInt("ID Indices", mesh->id_indices);

	component.AddInt("Num normals", mesh->num_normals);
	//component.AddInt("ID Normals", mesh->id_normals);

	component.AddInt("Num Texture Coords", mesh->num_tex_coords);
	//component.AddInt("ID Coords", mesh->id_coords);
}

void M_FileSystem::SaveMaterial(JsonObj component, GameObject* gameObject)
{
	MaterialComponent* material = gameObject->GetMaterialComponent();

	component.AddInt("UID", material->UID);
	component.AddString("TYPE", "Material");
	component.AddString("Name", material->name.c_str());
	component.AddString("Path", material->path.c_str());
	component.AddInt("Width", material->width);
	component.AddInt("Height", material->height);
	component.AddInt("BPP", material->bpp);

	component.AddInt("dataTexture", int(material->dataTexture));

	component.AddInt("buffer Data", material->bufferTexture);

	component.AddInt("Size", material->size);
	component.AddBool("Using Checker", material->useChecker);
}

void M_FileSystem::SaveTransform(JsonObj component, GameObject* gameObject)
{
	TransformComponent* transform = gameObject->GetTransformComponent();
	component.AddString("TYPE", "Transform");
	component.AddInt("UID", transform->UID);
	JsonArray position = component.AddArray("Position");
	JsonArray scale = component.AddArray("Scale");
	JsonArray rotation = component.AddArray("Rotation");

	position.AddFloat3(transform->position.x, transform->position.y, transform->position.z);
	scale.AddFloat3(transform->scale.x, transform->scale.y, transform->scale.z);
	rotation.AddQuaternion(transform->rotation.w, transform->rotation.x, transform->rotation.y, transform->rotation.z);
}

//  ----------------------------------------------------------------------------------------------
//  --------------------------------------------COMPONENTS LOADING--------------------------------
//  ----------------------------------------------------------------------------------------------

void M_FileSystem::LoadMesh(JsonObj components_iterator, GameObject* gameObject)
{
	MeshComponent* loadedMesh = new MeshComponent();
	loadedMesh->type = ComponentType::Mesh;
	loadedMesh->UID = components_iterator.GetInt("UID");
	loadedMesh->name = components_iterator.GetString("Name");
	loadedMesh->path = components_iterator.GetString("Path");


	loadedMesh->num_vertices = components_iterator.GetInt("Num vertices");
	//loadedMesh->id_vertices = components_iterator.GetInt("ID Vertices");

	loadedMesh->num_indices = components_iterator.GetInt("Num indices");
	//loadedMesh->id_indices = components_iterator.GetInt("ID Indices");

	loadedMesh->num_normals = components_iterator.GetInt("Num normals");
	//loadedMesh->id_normals = components_iterator.GetInt("ID Normals");

	loadedMesh->num_tex_coords = components_iterator.GetInt("Num Texture Coords");
	//loadedMesh->id_coords = components_iterator.GetInt("ID Coords");

	loadedMesh->CreateAABB();
	//Load vertices,indices... from mesh File
	std::string UID_string = std::to_string(components_iterator.GetInt("UID"));
	std::string bufferPath = App->geometry->meshesPath + UID_string;
	ReadFile(bufferPath.c_str(), &loadedMesh->meshBuffer);
	App->geometry->LoadOurMesh(loadedMesh->meshBuffer, loadedMesh);

	gameObject->AddComponent(loadedMesh);
	App->geometry->CreateBuffer(loadedMesh);
}

void M_FileSystem::LoadMaterial(JsonObj components_iterator, GameObject* gameObject)
{
	MaterialComponent* loadedMaterial = new MaterialComponent();
	App->geometry->CreateTextureBuffer(loadedMaterial);
	loadedMaterial->type = ComponentType::Material;
	loadedMaterial->size = components_iterator.GetInt("Size");
	loadedMaterial->UID = components_iterator.GetInt("UID");
	loadedMaterial->name = components_iterator.GetString("Name");
	loadedMaterial->path = components_iterator.GetString("Path");
	loadedMaterial->useChecker = components_iterator.GetBool("Using Checker");


	std::string UID_string = std::to_string(components_iterator.GetInt("UID"));
	std::string bufferPath = App->geometry->texturesPath + UID_string;

	ReadFile(bufferPath.c_str(), &loadedMaterial->materialBuffer);
	App->geometry->LoadOurMaterial(loadedMaterial->materialBuffer, loadedMaterial, loadedMaterial->size);
	loadedMaterial->bufferTexture = components_iterator.GetInt("buffer Data");

	App->geometry->LoadTexture(loadedMaterial->path.c_str(), loadedMaterial);

	gameObject->AddComponent(loadedMaterial);
}

void M_FileSystem::LoadTransform(JsonObj components_iterator, GameObject* gameObject)
{
	TransformComponent* loadedTransform = new TransformComponent();
	loadedTransform->type = ComponentType::Transform;
	loadedTransform->UID = components_iterator.GetInt("UID");

	JsonArray position = components_iterator.GetArray("Position");
	loadedTransform->position = position.GetFloat3(0);

	JsonArray scale = components_iterator.GetArray("Scale");
	loadedTransform->scale = scale.GetFloat3(0);

	JsonArray rotation = components_iterator.GetArray("Rotation");
	loadedTransform->rotation = rotation.GetQuaternion(0);

	gameObject->AddComponent(loadedTransform);
}

unsigned int M_FileSystem::Load(const char * path, const char * file, char ** buffer) const
{
	std::string full_path(path);
	full_path += file;
	return Load(full_path.c_str(), buffer);
}

// Read a whole file and put it in a new buffer
uint M_FileSystem::Load(const char* file, char** buffer) const
{
	uint ret = 0;

	PHYSFS_file* fs_file = PHYSFS_openRead(file);

	if (fs_file != nullptr)
	{
		PHYSFS_sint32 size = (PHYSFS_sint32)PHYSFS_fileLength(fs_file);

		if (size > 0)
		{
			*buffer = new char[size+1];
			uint readed = (uint)PHYSFS_read(fs_file, *buffer, 1, size);
			if (readed != size)
			{
				LOG("File System error while reading from file %s: %s\n", file, PHYSFS_getLastError());
				RELEASE_ARRAY(buffer);
			}
			else
			{
				ret = readed;
				//Adding end of file at the end of the buffer. Loading a shader file does not add this for some reason
				(*buffer)[size] = '\0';
			}
		}

		if (PHYSFS_close(fs_file) == 0)
			LOG("File System error while closing file %s: %s\n", file, PHYSFS_getLastError());
	}
	else
		LOG("File System error while opening file %s: %s\n", file, PHYSFS_getLastError());

	return ret;
}

bool M_FileSystem::DuplicateFile(const char* file, const char* dstFolder, std::string& relativePath)
{
	std::string fileStr, extensionStr;
	SplitFilePath(file, nullptr, &fileStr, &extensionStr);

	relativePath = relativePath.append(dstFolder).append("/") + fileStr.append(".") + extensionStr;
	std::string finalPath = std::string(*PHYSFS_getSearchPath()).append("/") + relativePath;

	return DuplicateFile(file, finalPath.c_str());

}

bool M_FileSystem::DuplicateFile(const char* srcFile, const char* dstFile)
{
	//TODO: Compare performance to calling Load(srcFile) and then Save(dstFile)
	std::ifstream src;
	src.open(srcFile, std::ios::binary);
	bool srcOpen = src.is_open();
	std::ofstream  dst(dstFile, std::ios::binary);
	bool dstOpen = dst.is_open();

	dst << src.rdbuf();

	src.close();
	dst.close();

	if (srcOpen && dstOpen)
	{
		LOG("[success] File Duplicated Correctly");
		return true;
	}
	else
	{
		LOG("[error] File could not be duplicated");
		return false;
	}
}

int close_sdl_rwops(SDL_RWops *rw)
{
	RELEASE_ARRAY(rw->hidden.mem.base);
	SDL_FreeRW(rw);
	return 0;
}

// Save a whole buffer to disk
uint M_FileSystem::Save(const char* file, const void* buffer, unsigned int size, bool append) const
{
	unsigned int ret = 0;

	bool overwrite = PHYSFS_exists(file) != 0;
	PHYSFS_file* fs_file = (append) ? PHYSFS_openAppend(file) : PHYSFS_openWrite(file);

	if (fs_file != nullptr)
	{
		uint written = (uint)PHYSFS_write(fs_file, (const void*)buffer, 1, size);
		if (written != size)
		{
			LOG("[error] File System error while writing to file %s: %s", file, PHYSFS_getLastError());
		}
		else
		{
			if (append == true)
			{
				LOG("Added %u data to [%s%s]", size, GetWriteDir(), file);
			}
			else if (overwrite == true)
			{
				LOG("File [%s%s] overwritten with %u bytes", GetWriteDir(), file, size);
			}
			else
			{
				LOG("New file created [%s%s] of %u bytes", GetWriteDir(), file, size);
			}

			ret = written;
		}

		if (PHYSFS_close(fs_file) == 0)
			LOG("[error] File System error while closing file %s: %s", file, PHYSFS_getLastError());
	}
	else
		LOG("[error] File System error while opening file %s: %s", file, PHYSFS_getLastError());

	return ret;
}

uint M_FileSystem::GetLastModTime(const char* filename)
{
	return PHYSFS_getLastModTime(filename);
}

std::string M_FileSystem::GetUniqueName(const char* path, const char* name) const
{
	//TODO: modify to distinguix files and dirs?
	std::vector<std::string> files, dirs;
	DiscoverFiles(path, files, dirs);

	std::string finalName(name);
	bool unique = false;

	for (uint i = 0; i < 50 && unique == false; ++i)
	{
		unique = true;

		//Build the compare name (name_i)
		if (i > 0)
		{
			finalName = std::string(name).append("_");
			if (i < 10)
				finalName.append("0");
			finalName.append(std::to_string(i));
		}

		//Iterate through all the files to find a matching name
		for (uint f = 0; f < files.size(); ++f)
		{
			if (finalName == files[f])
			{
				unique = false;
				break;
			}
		}
	}
	return finalName;
}

