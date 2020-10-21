#include "Globals.h"
#include "Application.h"
#include "ModuleGeometry.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "ModuleSceneIntro.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

struct aiLogStream stream;

ModuleGeometry::ModuleGeometry(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleGeometry::~ModuleGeometry()
{
}

bool ModuleGeometry::Start()
{
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return true;
}

update_status ModuleGeometry::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleGeometry::Update(float dt)
{

	App->scene_intro->Draw();
	return UPDATE_CONTINUE;
}

update_status ModuleGeometry::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleGeometry::LoadFbx(const char* path)
{
	Mesh* ourMesh = new Mesh;

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes()) 
	{
		//iterate meshes
		for (uint i = 0; i < scene->mNumMeshes; i++)
		{
			
			//copy vertices
			aiMesh* aimesh = scene->mMeshes[i];
			ourMesh->num_vertices = aimesh->mNumVertices;
			ourMesh->vertices = new float[ourMesh->num_vertices * 3];
			memcpy(ourMesh->vertices, aimesh->mVertices, sizeof(float) * ourMesh->num_vertices * 3);
			LOG("New mesh with %d vertices", ourMesh->num_vertices);

			//copy faces
			if (aimesh->HasFaces()) 
			{
				ourMesh->num_indices = aimesh->mNumFaces * 3;
				ourMesh->indices = new uint[ourMesh->num_indices];
				for (uint i = 0; i < aimesh->mNumFaces; i++)
				{
					if (aimesh->mFaces[i].mNumIndices != 3) {
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else {
						memcpy(&ourMesh->indices[i * 3], aimesh->mFaces[i].mIndices, 3 * sizeof(uint));
					}
				}
			}


			if (aimesh->HasNormals())
			{
				ourMesh->num_normals = ourMesh->num_vertices;
				ourMesh->normals = new float[ourMesh->num_normals * 3];
				memcpy(ourMesh->normals, aimesh->mNormals, sizeof(float) * ourMesh->num_normals * 3);
				LOG("New mesh with %d normals", ourMesh->num_normals);
			}
		}

		ourMeshes.push_back(ourMesh);
		App->scene_intro->CreateBuffer(ourMesh);

		//Release assimp
		aiReleaseImport(scene);


	}
	else {
		LOG("Error loading scene %s", path);
		App->editor->AddLog("Error loading scene");
	}

	return true;
}

bool ModuleGeometry::CleanUp()
{
	aiDetachAllLogStreams();

	for (uint i = 0; i < ourMeshes.size(); i++)
	{
		delete ourMeshes[i];
	}

	return true;
}
