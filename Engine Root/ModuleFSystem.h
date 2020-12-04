#ifndef __MODULEFILESYSTEM_H__
#define __MODULEFILESYSTEM_H__

#include "Module.h"
#include <vector>
#include <string>


#define LIBRARY_PATH "Library/"
#define FOLDERS_PATH "Library/Folders/"
#define MESHES_PATH "Library/Meshes/"
#define MATERIALS_PATH "Library/Materials/"
#define TEXTURES_PATH "Library/Textures/"
#define MODELS_PATH "Library/Models/"
#define ANIMATIONS_PATH "Library/Animations/"
#define BONES_PATH "Library/Bones/"
#define PARTICLES_PATH "Library/ParticleSystems/"
#define SHADERS_PATH "Library/Shaders/"
#define SCENES_PATH "Library/Scenes/"

struct SDL_RWops;
int close_sdl_rwops(SDL_RWops *rw);

struct aiFileIO;

//struct BASS_FILEPROCS;
class Config;
struct PathNode;

class JsonObj;
class JsonArray;
class GameObject;


//DELETE AFTER

class M_FileSystem : public Module
{
public:

	M_FileSystem(Application* app, bool start_enabled = true);// const char* game_path = nullptr);

	// Destructor
	~M_FileSystem();

	// Called before render is available
	bool Init() ;

	// Called before quitting
	bool CleanUp() override;

	void CreateLibraryDirectories();

	// Utility functions
	bool AddPath(const char* path_or_zip);
	bool Exists(const char* file) const;
	uint WriteFile(const char* file, char* buffer, uint size);
	uint ReadFile(const char* file, char** buffer);
	bool CreateDir(const char* dir);
	bool IsDirectory(const char* file) const;
	const char* GetWriteDir() const;
	void DiscoverFiles(const char* directory, std::vector<std::string>& file_list, std::vector<std::string>& dir_list) const;
	void GetAllFilesWithExtension(const char* directory, const char* extension, std::vector<std::string>& file_list) const;
	void GetRealDir(const char* path, std::string& output) const;
	std::string GetPathRelativeToAssets(const char* originalPath) const;

	bool HasExtension(const char* path) const;
	bool HasExtension(const char* path, std::string extension) const;
	bool HasExtension(const char* path, std::vector<std::string> extensions) const;

	std::string NormalizePath(const char* path) const;
	void SplitFilePath(const char* full_path, std::string* path, std::string* file = nullptr, std::string* extension = nullptr) const;
	void DetectExtension(std::string path, std::string file, std::string extension ) const;
	void CreatePrimitives(std::string path, std::string file);

	void SaveScene(char** sceneBuffer);
	JsonArray SaveGameObjects(JsonObj scene);
	JsonArray SaveSceneMode(JsonObj scene);
	void SaveGobjsChilds(GameObject* gameObject, JsonObj JsonGob);
	void SaveGobjsComponentes(GameObject* gameObject, JsonObj JsonGob);
	void SaveMesh(JsonObj component, GameObject* gameObject);
	void SaveMaterial(JsonObj component, GameObject* gameObject);
	void SaveTransform(JsonObj component, GameObject* gameObject);

	void LoadScene(char* sceneBuffer);
	void LoadSceneMode(JsonObj* scene);
	GameObject* LoadGameObjects(JsonObj current_node);
	void LoadGobjsChilds(GameObject* gameObject, JsonObj current_node);
	void LoadGobjsComponents(GameObject* gameObject, JsonObj current_node);
	void LoadMesh(JsonObj component, GameObject* gameObject);
	void LoadMaterial(JsonObj component, GameObject* gameObject);
	void LoadTransform(JsonObj component, GameObject* gameObject);
	// Open for Read/Write
	unsigned int Load(const char* path, const char* file, char** buffer) const;
	unsigned int Load(const char* file, char** buffer) const;

	bool DuplicateFile(const char* file, const char* dstFolder, std::string& relativePath);
	bool DuplicateFile(const char* srcFile, const char* dstFile);

	unsigned int Save(const char* file, const void* buffer, unsigned int size, bool append = false) const;

	uint GetLastModTime(const char* filename);
	std::string GetUniqueName(const char* path, const char* name) const;
};

#endif // __MODULEFILESYSTEM_H__
