#pragma once
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"
#include "Json.h"
#include <vector>

#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"


#include <gl/GL.h>
#include <gl/GLU.h>

#include "GameObject.h"

class JsonObj;
struct MeshComponent;
class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnClickSelection(const LineSegment& segment);
	void DrawRay(LineSegment& segment);

	//Save and Load Functions
	void SaveScene(char** sceneBuffer);
	JsonArray SaveGameObjects(JsonObj scene);
	JsonArray SaveSceneMode(JsonObj scene);


	void SaveGobjsChilds(GameObject* gameObject, JsonArray GobsArray);
	void SaveGobjsComponentes(GameObject* gameObject, JsonObj JsonGob);
	void SaveMesh(JsonObj component, GameObject* gameObject);
	void SaveMaterial(JsonObj component, GameObject* gameObject);
	void SaveTransform(JsonObj component, GameObject* gameObject);
	void SaveCameraComponent(JsonObj component, GameObject* gameObject);

	void LoadScene(char* sceneBuffer);
	void LoadSceneMode(JsonObj* scene);
	GameObject* LoadGameObjects(JsonObj current_node);
	void LoadGobjsChilds(GameObject* gameObject, JsonObj current_node);
	void LoadGobjsComponents(GameObject* gameObject, JsonObj current_node);
	void LoadMesh(JsonObj component, GameObject* gameObject);
	void LoadMaterial(JsonObj component, GameObject* gameObject);
	void LoadTransform(JsonObj component, GameObject* gameObject);
	void DeleteAssetNResource(std::string fileDir);

	GameObject* GetGameObjectbyUID(int UID);
public:
	GameObject* selected=nullptr;
	GameObject* toSelect = nullptr;
	GameObject* CreateGameObject(std::string name);
	TransformComponent* cameraTransform;
	std::vector<GameObject*> gameObjectsList;
	JsonObj myScene;
	char* sceneBuffer;

	bool showDebugAABB;

};
