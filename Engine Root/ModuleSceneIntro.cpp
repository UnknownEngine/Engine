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

	showDebugAABB = true;

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

	for (uint i = 0; i < candidates.size(); i++)
	{

		//Testing triangle by triangle
		const MeshComponent* mesh = candidates[i]->GetMeshComponent();
		if (mesh)
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
						_toSelect = candidates[i];
						break;
					}
				}
			
		}
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



