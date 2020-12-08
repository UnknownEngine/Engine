#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "ModuleWindow.h"


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

	App->camera->UpdateCameraPos(float3(-7.0f, 3.0f, 0.0f));
	App->camera->LookAt(float3(0, 0, 0));

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

	if (gameObjectsList.size() > 0) {
		for (uint i = 0; i < gameObjectsList.size(); i++)
		{
			for (uint k = 0; k < gameObjectsList[i]->childs.size(); k++)
			{
				float3 corners[8];
				gameObjectsList[i]->childs[k]->GetAABB().GetCornerPoints(corners);

				glColor3f(0, 1, 0);
				glBegin(GL_LINES);
				for (uint j = 0; j < 8; j++)
				{
					glVertex3f(corners[j].x, corners[j].y, corners[j].z);
				}


				glEnd();
				glColor3f(1, 1, 1);
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
	selected = nullptr;
	std::vector<GameObject*> candidates;
	for (uint i = 0; i < gameObjectsList.size(); i++)
	{
		gameObjectsList[i]->UpdateAABB();
		for (uint j = 0; j < gameObjectsList[i]->childs.size(); j++)
		{
			gameObjectsList[i]->childs[j]->UpdateAABB();
		}
	}
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
		}
	}



	//GameObject* toSelect = nullptr;
	//for (uint i = 0; i < candidates.size() && toSelect == nullptr; i++)
	//{

	//	//Testing triangle by triangle
	//	const MeshComponent* mesh = candidates[i]->GetMeshComponent();
	//	if (mesh)
	//	{
	//			LineSegment local = segment;
	//			local.Transform(candidates[i]->GetTransformComponent()->global_transform.Inverted());
	//			for (uint v = 0; v < mesh->num_indices; v += 3)
	//			{
	//				uint indexA = mesh->indices[v] * 3;
	//				float3 a(&mesh->vertices[indexA]);

	//				uint indexB = mesh->indices[v + 1] * 3;
	//				float3 b(&mesh->vertices[indexB]);

	//				uint indexC = mesh->indices[v + 2] * 3;
	//				float3 c(&mesh->vertices[indexC]);

	//				Triangle triangle(a, b, c);

	//				if (local.Intersects(triangle, nullptr, nullptr))
	//				{
	//					toSelect = candidates[i];
	//					break;
	//				}
	//			}
	//		
	//	}
	//}
	if (candidates.size() > 0)
		selected = candidates[0];

}



