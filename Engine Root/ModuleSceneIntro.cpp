#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "ModuleWindow.h"
#include "Json.h"


#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#define CHECKERS_HEIGHT 8
#define CHECKERS_WIDTH 8

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->LookAt(float3(0, 0, 0));

	showDebugAABB = false;

	gameObjectsList.push_back(App->camera->cameraObject);
	//App->geometry->LoadFbx(buffer, size, file, file);
	//char* drop_file_dir = "Assets/FBXs/BakerHouse.fbx";
	//std::string  path = "";
	//std::string  file = "";
	//std::string  extension = "";
	//App->fsystem->SplitFilePath(drop_file_dir, &path, &file, &extension);
	//App->fsystem->DetectExtension(path, file, extension);
	//App->fsystem->LoadScene(App->scene_intro->sceneBuffer);
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

GameObject* ModuleSceneIntro::CreateGameObject(std::string name)
{

	return nullptr;
}


// Update: draw background
update_status ModuleSceneIntro::Update(float dt)
{

	Planetest p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	//TODO: Set this recursive
	if (showDebugAABB) {
		if (gameObjectsList.size() > 0) {
			for (uint i = 0; i < gameObjectsList.size(); i++)
			{
				for (uint j = 0; j < gameObjectsList[i]->childs.size(); j++)
				{
					if (gameObjectsList[i]->childs[j]->GetMeshComponent()->r_mesh != nullptr)
					{
						float3 corners[8];
						gameObjectsList[i]->childs[j]->GetAABB().GetCornerPoints(corners);

						glColor3f(0, 1, 0);
						glBegin(GL_LINES);

						//Botton face
						glVertex3f(corners[0].x, corners[0].y, corners[0].z);
						glVertex3f(corners[1].x, corners[1].y, corners[1].z);
						glVertex3f(corners[1].x, corners[1].y, corners[1].z);
						glVertex3f(corners[5].x, corners[5].y, corners[5].z);
						glVertex3f(corners[5].x, corners[5].y, corners[5].z);
						glVertex3f(corners[4].x, corners[4].y, corners[4].z);
						glVertex3f(corners[4].x, corners[4].y, corners[4].z);
						glVertex3f(corners[0].x, corners[0].y, corners[0].z);

						//Top Face
						glVertex3f(corners[2].x, corners[2].y, corners[2].z);
						glVertex3f(corners[3].x, corners[3].y, corners[3].z);
						glVertex3f(corners[3].x, corners[3].y, corners[3].z);
						glVertex3f(corners[7].x, corners[7].y, corners[7].z);
						glVertex3f(corners[7].x, corners[7].y, corners[7].z);
						glVertex3f(corners[6].x, corners[6].y, corners[6].z);
						glVertex3f(corners[6].x, corners[6].y, corners[6].z);
						glVertex3f(corners[2].x, corners[2].y, corners[2].z);

						//Left Face
						glVertex3f(corners[1].x, corners[1].y, corners[1].z);
						glVertex3f(corners[3].x, corners[3].y, corners[3].z);
						glVertex3f(corners[0].x, corners[0].y, corners[0].z);
						glVertex3f(corners[2].x, corners[2].y, corners[2].z);


						//Right Face
						glVertex3f(corners[5].x, corners[5].y, corners[5].z);
						glVertex3f(corners[7].x, corners[7].y, corners[7].z);
						glVertex3f(corners[4].x, corners[4].y, corners[4].z);
						glVertex3f(corners[6].x, corners[6].y, corners[6].z);

						glEnd();
						glColor3f(1, 1, 1);

						if (gameObjectsList[i]->childs[j]->childs.size() > 0) {
							for (uint k = 0; k < gameObjectsList[i]->childs[j]->childs.size(); k++) {
								float3 corners[8];
								gameObjectsList[i]->childs[j]->childs[k]->GetAABB().GetCornerPoints(corners);

								glColor3f(0, 1, 0);
								glBegin(GL_LINES);

								//Botton face
								glVertex3f(corners[0].x, corners[0].y, corners[0].z);
								glVertex3f(corners[1].x, corners[1].y, corners[1].z);
								glVertex3f(corners[1].x, corners[1].y, corners[1].z);
								glVertex3f(corners[5].x, corners[5].y, corners[5].z);
								glVertex3f(corners[5].x, corners[5].y, corners[5].z);
								glVertex3f(corners[4].x, corners[4].y, corners[4].z);
								glVertex3f(corners[4].x, corners[4].y, corners[4].z);
								glVertex3f(corners[0].x, corners[0].y, corners[0].z);

								//Top Face
								glVertex3f(corners[2].x, corners[2].y, corners[2].z);
								glVertex3f(corners[3].x, corners[3].y, corners[3].z);
								glVertex3f(corners[3].x, corners[3].y, corners[3].z);
								glVertex3f(corners[7].x, corners[7].y, corners[7].z);
								glVertex3f(corners[7].x, corners[7].y, corners[7].z);
								glVertex3f(corners[6].x, corners[6].y, corners[6].z);
								glVertex3f(corners[6].x, corners[6].y, corners[6].z);
								glVertex3f(corners[2].x, corners[2].y, corners[2].z);

								//Left Face
								glVertex3f(corners[1].x, corners[1].y, corners[1].z);
								glVertex3f(corners[3].x, corners[3].y, corners[3].z);
								glVertex3f(corners[0].x, corners[0].y, corners[0].z);
								glVertex3f(corners[2].x, corners[2].y, corners[2].z);


								//Right Face
								glVertex3f(corners[5].x, corners[5].y, corners[5].z);
								glVertex3f(corners[7].x, corners[7].y, corners[7].z);
								glVertex3f(corners[4].x, corners[4].y, corners[4].z);
								glVertex3f(corners[6].x, corners[6].y, corners[6].z);

								glEnd();
								glColor3f(1, 1, 1);
							}
						}
					}
				}


			}
		}
	}
	if (App->renderer3D->gl_wireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnClickSelection(const LineSegment& segment)
{
	GameObject* _toSelect = nullptr;
	std::vector<GameObject*> candidates;

	//TODO: Set this recursive
	for (uint i = 0; i < gameObjectsList.size(); i++)
	{
		for (uint j = 0; j < gameObjectsList[i]->childs.size(); j++)
		{
			if (segment.Intersects(gameObjectsList[i]->childs[j]->GetAABB()))
			{
				float hit_near, hit_far;
				if (segment.Intersects(gameObjectsList[i]->childs[j]->GetOBB(), hit_near, hit_far))
					candidates.push_back(gameObjectsList[i]->childs[j]);
			}
			if (gameObjectsList[i]->childs[j]->childs.size() > 0) {
				for (uint k = 0; k < gameObjectsList[i]->childs[j]->childs.size(); k++)
				{
					if (segment.Intersects(gameObjectsList[i]->childs[j]->childs[k]->GetAABB()))
					{
						float hit_near, hit_far;
						if (segment.Intersects(gameObjectsList[i]->childs[j]->childs[k]->GetOBB(), hit_near, hit_far))
							candidates.push_back(gameObjectsList[i]->childs[j]->childs[k]);
					}
				}
			}
		}
	}

	std::vector<GameObject*> real_candidates;

	for (uint i = 0; i < candidates.size(); i++)
	{

		//Testing triangle by triangle
		const MeshComponent* mesh = candidates[i]->GetMeshComponent();
		if (mesh && mesh->r_mesh != nullptr)
		{
				LineSegment local = segment;
				local.Transform(candidates[i]->GetTransformComponent()->global_transform.Inverted());
				for (uint v = 0; v < mesh->r_mesh->num_indices; v += 3)
				{
					uint indexA = mesh->r_mesh->indices[v] * 3;
					float3 a(&mesh->r_mesh->vertices[indexA]);

					uint indexB = mesh->r_mesh->indices[v + 1] * 3;
					float3 b(&mesh->r_mesh->vertices[indexB]);

					uint indexC = mesh->r_mesh->indices[v + 2] * 3;
					float3 c(&mesh->r_mesh->vertices[indexC]);

					Triangle triangle(a, b, c);

					if (local.Intersects(triangle, nullptr, nullptr))
					{
						real_candidates.push_back(candidates[i]);
						break;
					}
				}
			
		}
	}
	if (real_candidates.size() > 0) {
		int index = 0;
		float distance = segment.a.Distance(real_candidates[0]->GetAABB().CenterPoint());
		for (uint i = 0; i < real_candidates.size(); i++)
		{
			if (segment.a.Distance(real_candidates[i]->GetAABB().CenterPoint()) < distance) {
				distance = segment.a.Distance(real_candidates[i]->GetAABB().CenterPoint());
				index = i;
			}
		}
		_toSelect = real_candidates[index];
	}
	selected = _toSelect;

}

void ModuleSceneIntro::DrawRay(LineSegment& segment)
{
		glColor3f(1, 0, 0);
		glBegin(GL_LINES);

		glVertex3f(segment.a.x, segment.a.y, segment.a.z);
		glVertex3f(segment.b.x, segment.b.y, segment.b.z);

		glEnd();
		glColor3f(1, 1, 1);
	
}


//  ----------------------------------------------------------------------------------------------
//  --------------------------------------------SAVE----------------------------------------------
//  ----------------------------------------------------------------------------------------------

void ModuleSceneIntro::SaveScene(char** sceneBuffer)
{
	JsonObj scene;
	SaveSceneMode(scene);
	if (!gameObjectsList.empty())
	{
		scene.AddArray("GameObjects");
		SaveGameObjects(scene);
	}
	else
	{
		LOG("Any gameObjects to be saved");
	}
	uint size = scene.Save(sceneBuffer);
	App->fsystem->WriteFile("Library/Config.json", *sceneBuffer, size);

}

JsonArray ModuleSceneIntro::SaveGameObjects(JsonObj scene)
{
	JsonArray gameObjects = scene.GetArray("GameObjects");

	for (int it = 0; it < gameObjectsList.size(); it++)
	{
		JsonObj gameObject;
		gameObjects.AddObject(gameObject);
		gameObject.AddInt("UID", gameObjectsList.at(it)->UID);
		gameObject.AddInt("Parent UID", gameObjectsList.at(it)->ParentUID);
		gameObject.AddString("Name", gameObjectsList.at(it)->nameID.c_str());
		gameObject.AddBool("Active",gameObjectsList.at(it)->active);
		if (!App->scene_intro->gameObjectsList.at(it)->childs.empty())
		{
			SaveGobjsChilds(gameObjectsList.at(it), gameObjects);
		}
		if (!gameObjectsList.at(it)->components.empty())
		{
			SaveGobjsComponentes(gameObjectsList.at(it), gameObject);
		}
	}
	return gameObjects;
}

JsonArray ModuleSceneIntro::SaveSceneMode(JsonObj scene)
{
	JsonArray sceneModes = scene.AddArray("Scene Mode");
	JsonObj mode;
	sceneModes.AddObject(mode);

	mode.AddBool("EditorMode", App->editorMode);
	mode.AddBool("GameMode", App->gameMode);
	mode.AddBool("Paused", App->gameModePaused);

	return sceneModes;
}

void ModuleSceneIntro::SaveGobjsChilds(GameObject* gameObject, JsonArray GobsArray)
{
	for (int i = 0; i < gameObject->childs.size(); i++)
	{
		JsonObj child;
		GobsArray.AddObject(child);
		child.AddInt("UID", gameObject->childs.at(i)->UID);
		child.AddInt("Parent UID", gameObject->childs.at(i)->ParentUID);
		child.AddString("Name", gameObject->childs.at(i)->nameID.c_str());
		child.AddBool("Active", gameObject->childs.at(i)->active);
		if (!gameObject->childs.at(i)->components.empty())
		{
			SaveGobjsComponentes(gameObject->childs.at(i), child);
		}
	}
}

void ModuleSceneIntro::SaveGobjsComponentes(GameObject* gameObject, JsonObj JsonGob)
{
	JsonArray components = JsonGob.AddArray("Components");
	for (int i = 0; i < gameObject->components.size(); ++i)
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
		if (gameObject->components.at(i)->type == ComponentType::Camera)
		{
			SaveCameraComponent(component, gameObject);
		}
	}
}

void ModuleSceneIntro::SaveMesh(JsonObj component, GameObject* gameObject)
{
	MeshComponent* mesh = gameObject->GetMeshComponent();
	component.AddInt("UID", mesh->UID);
	component.AddString("TYPE", "Mesh");
}

void ModuleSceneIntro::SaveMaterial(JsonObj component, GameObject* gameObject)
{
	MaterialComponent* material = gameObject->GetMaterialComponent();
	if (material->r_texture != NULL)
	{
		component.AddInt("UID", material->UID);
		component.AddString("TYPE", "Material");
		component.AddString("Asset path", material->r_texture->path.c_str());
	}
}

void ModuleSceneIntro::SaveTransform(JsonObj component, GameObject* gameObject)
{
	TransformComponent* transform = gameObject->GetTransformComponent();
	component.AddString("TYPE", "Transform");
	JsonArray position = component.AddArray("Position");
	JsonArray scale = component.AddArray("Scale");
	JsonArray rotation = component.AddArray("Rotation");

	position.AddFloat3(transform->position.x, transform->position.y, transform->position.z);
	scale.AddFloat3(transform->scale.x, transform->scale.y, transform->scale.z);
	rotation.AddQuaternion(transform->rotation.w, transform->rotation.x, transform->rotation.y, transform->rotation.z);
}

void ModuleSceneIntro::SaveCameraComponent(JsonObj component, GameObject* gameObject)
{
	CameraComponent* camera = gameObject->GetCameraComponent();
	component.AddString("TYPE", "Camera");
	component.AddInt("Frustum Type", camera->frustum.type);
	component.AddFloat("Frustum Near Distance", camera->frustum.nearPlaneDistance);
	component.AddFloat("Frustum Far Distance", camera->frustum.farPlaneDistance);
	component.AddFloat("Frustum Vertical FOV", camera->frustum.verticalFov);

	JsonArray frustumFront = component.AddArray("Frustum Front");
	frustumFront.AddFloat3(camera->frustum.front.x, camera->frustum.front.y, camera->frustum.front.z);

	JsonArray frustumUp = component.AddArray("Frustum Up");
	frustumUp.AddFloat3(camera->frustum.up.x, camera->frustum.up.y, camera->frustum.up.z);

	JsonArray frustumPos = component.AddArray("Frustum Pos");
	frustumPos.AddFloat3(camera->frustum.pos.x, camera->frustum.pos.y, camera->frustum.pos.z);

}

void ModuleSceneIntro::LoadScene(char* sceneBuffer)
{
	gameObjectsList.clear();
	App->fsystem->ReadFile("Library/Config.json", &sceneBuffer);
	JsonObj* myScene = new JsonObj(sceneBuffer);

	if (App->onStart)
	{
		LoadSceneMode(myScene);
		App->onStart = false;
	}

	JsonArray gameObjects = myScene->GetArray("GameObjects");
	for (int i = 0; i < gameObjects.Size(); i++)
	{
		JsonObj object = gameObjects.GetObjectAt(i);

		GameObject* created_gameobject = LoadGameObjects(object);

		if (object.GetArray("Components") != NULL)
		{
			LoadGobjsComponents(created_gameobject, object);
		}
		if (created_gameobject->ParentUID != 0)
		{
			GetGameObjectbyUID(created_gameobject->ParentUID)->childs.push_back(created_gameobject);
		}
		else
		{
			App->scene_intro->gameObjectsList.push_back(created_gameobject);
		}
	}
}

void ModuleSceneIntro::LoadSceneMode(JsonObj* scene)
{
	JsonArray mode = scene->GetArray("Scene Mode");

	for (int i = 0; i < mode.Size(); i++)
	{
		JsonObj node = mode.GetObjectAt(i);
		App->editorMode = node.GetBool("EditorMode");
		App->gameMode = node.GetBool("GameMode");
		App->gameModePaused = node.GetBool("Paused");
	}
}

GameObject* ModuleSceneIntro::LoadGameObjects(JsonObj current_node)
{
	GameObject* gameObject = new GameObject(current_node.GetString("Name"), current_node.GetInt("UID"));

	gameObject->active = current_node.GetBool("Active");
	gameObject->nameID = current_node.GetString("Name");
	gameObject->UID = current_node.GetInt("UID");
	gameObject->ParentUID = current_node.GetInt("Parent UID");

	return gameObject;
}

void ModuleSceneIntro::LoadGobjsChilds(GameObject* gameObject, JsonObj current_node)
{
}

void ModuleSceneIntro::LoadGobjsComponents(GameObject* gameObject, JsonObj current_node)
{
	JsonArray components_array = current_node.GetArray("Components");
	JsonObj components_iterator;
	for (int i = 0; i < components_array.Size(); ++i)
	{
		components_iterator = components_array.GetObjectAt(i);

		std::string type = components_iterator.GetString("TYPE");
		if (type == "Mesh")
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
		else if (type == "Camera") {
			CameraComponent* c_camera = new CameraComponent();
			gameObject->AddComponent(c_camera);
			App->camera->cameraObject = gameObject;
		}
	}
}

void ModuleSceneIntro::LoadMesh(JsonObj component, GameObject* gameObject)
{
	uint meshUID = component.GetInt("UID");
	MeshComponent* c_mesh = new MeshComponent;
	ResourceMesh* r_mesh = new ResourceMesh(meshUID, ResourceType::mesh);

	App->fsystem->ReadFile((App->resourceManager->meshesLibPath + std::to_string(meshUID)).c_str(), &r_mesh->meshBuffer);
	App->geometry->LoadOurMesh(r_mesh->meshBuffer, r_mesh);

	c_mesh->r_mesh = r_mesh;
	c_mesh->r_mesh->CreateAABB();
	c_mesh->UID = meshUID;

	App->geometry->CreateBuffer(c_mesh->r_mesh);

	gameObject->AddComponent(c_mesh);
}

void ModuleSceneIntro::LoadMaterial(JsonObj component, GameObject* gameObject)
{
	uint materialUID = component.GetInt("UID");
	std::string path = component.GetString("Asset path");

	if (gameObject->GetMaterialComponent() == nullptr)
	{
		MaterialComponent* c_mat = new MaterialComponent;
		c_mat->r_texture= App->resourceManager->LoadTexture(component);
		gameObject->AddComponent(c_mat);
	}
	else
	{
		gameObject->GetMaterialComponent()->r_texture = App->resourceManager->LoadTexture(component);
	}
	
}

void ModuleSceneIntro::LoadTransform(JsonObj components_iterator, GameObject* gameObject)
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

void ModuleSceneIntro::DeleteFBXnResource(std::string metaDir)
{
	char* metaBuffer;
	App->fsystem->ReadFile(metaDir.c_str(), &metaBuffer);
	
	JsonObj meta(metaBuffer);
	char* rootmodelBuffer;
	App->fsystem->ReadFile(meta.GetString("Library path"), &rootmodelBuffer);
	JsonObj rootModel(rootmodelBuffer);

	JsonArray childs = rootModel.GetArray("Childs UID");

	std::vector<int> ModelsUIDs = childs.GetUIDs(0);

	for (int i = 0; i < ModelsUIDs.size(); i++)
	{
		char* childBuffer;
		App->fsystem->ReadFile((App->resourceManager->modelsLibPath + std::to_string(ModelsUIDs.at(i))).c_str(), &childBuffer);
		JsonObj child(childBuffer);

		uint meshUID = child.GetInt("Mesh UID");
		std::string meshDir = App->resourceManager->meshesLibPath + std::to_string(meshUID);

		if (App->fsystem->CheckIfExists(meshDir))
		{
			r_meshesToDelete.push_back(meshUID);
			App->fsystem->Remove(meshDir.c_str());
		}
		App->fsystem->Remove((App->resourceManager->modelsLibPath + std::to_string(ModelsUIDs.at(i))).c_str());
	}
	App->fsystem->Remove(meta.GetString("Library path"));

	std::string fullPath = meta.GetString("Asset Path");

	App->fsystem->Remove(metaDir.c_str());
	App->fsystem->Remove(fullPath.c_str());
	App->resourceManager->fbxList.clear();
	App->resourceManager->SetFbxList();
	EmptyMeshComponents(r_meshesToDelete);

	r_meshesToDelete.clear();
}

void ModuleSceneIntro::DeleteTexturenResource(std::string metaDir)
{
	char* metaBuffer;
	App->fsystem->ReadFile(metaDir.c_str(), &metaBuffer);

	JsonObj meta(metaBuffer);
	char* textureBuffer;

	r_texturesToDelete.push_back(meta.GetInt("UID"));
	App->fsystem->Remove(meta.GetString("Library path"));
	App->fsystem->Remove(meta.GetString("Asset path"));
	App->fsystem->Remove(metaDir.c_str());

	App->resourceManager->textureList.clear();
	App->resourceManager->SetTexturesList();
	EmptyTextureComponents(r_texturesToDelete);

	r_texturesToDelete.clear();
}

void ModuleSceneIntro::EmptyMeshComponents(std::vector<int> meshestoDelete)
{
	for (int i = 0; i < meshestoDelete.size(); i++)
	{
		for (int j = 0; j < gameObjectsList.size(); j++)
		{
			if (gameObjectsList.at(j)->nameID != "Camera GameObject")
			{
			if (!gameObjectsList.at(j)->childs.empty())
			{
				for (int n = 0; n < gameObjectsList.at(j)->childs.size(); n++)
				{
					if (meshestoDelete.at(i) == gameObjectsList.at(j)->childs.at(n)->GetMeshComponent()->UID)
					{
						gameObjectsList.at(j)->childs.at(n)->GetMeshComponent()->r_mesh = nullptr;
						gameObjectsList.at(j)->childs.at(n)->GetMeshComponent()->UID = NULL;
					}
				}
			}
			else
			{
				if (meshestoDelete.at(i) == gameObjectsList.at(j)->GetMeshComponent()->UID)
				{
					if (gameObjectsList.at(i)->GetMeshComponent()->r_mesh != nullptr)
					{
						gameObjectsList.at(j)->GetMeshComponent()->r_mesh = nullptr;
						gameObjectsList.at(j)->GetMeshComponent()->UID = NULL;
					}
				}
			}
			}
		}
	}
}

void ModuleSceneIntro::EmptyTextureComponents(std::vector<int> texturestoDelete)
{
	for (int i = 0; i < texturestoDelete.size(); i++)
	{
		for (int j = 0; j < gameObjectsList.size(); j++)
		{
			if (!gameObjectsList.at(j)->childs.empty())
			{
				for (int n = 0; n < gameObjectsList.at(j)->childs.size(); n++)
				{
					if (texturestoDelete.at(i) == gameObjectsList.at(j)->childs.at(n)->GetMaterialComponent()->UID)
					{
						gameObjectsList.at(j)->childs.at(n)->GetMaterialComponent()->r_texture = nullptr;
						gameObjectsList.at(j)->childs.at(n)->GetMaterialComponent()->UID = NULL;
					}
				}
			}
			else
			{
				if (texturestoDelete.at(i) == gameObjectsList.at(j)->GetMaterialComponent()->UID)
				{
					gameObjectsList.at(j)->GetMaterialComponent()->r_texture = nullptr;
					gameObjectsList.at(j)->GetMaterialComponent()->UID = NULL;
				}
			}
		}
	}
}


GameObject* ModuleSceneIntro::GetGameObjectbyUID(int UID)
{
	for (int i = 0; i < gameObjectsList.size(); i++)
	{
		if (gameObjectsList.at(i)->UID == UID)
		{
			return gameObjectsList.at(i);
		}
	}
	return nullptr;
}




